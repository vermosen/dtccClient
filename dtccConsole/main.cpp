#ifndef MAIN_CPP_
#define MAIN_CPP_

#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <vector>

#include <boost/chrono.hpp>

#include "database/recordsets/tradeRecordset.hpp"
#include "database/records/tradeRecordGrammar.hpp"

#define TESTT
#ifdef TESTT

boost::chrono::high_resolution_clock::time_point start;

int main(int * argc, char ** argv)
{
	tradeRecordGrammar<std::string::const_iterator> g; // Our grammar

	start = boost::chrono::high_resolution_clock::now();
	int i = 0;  while (i++ < 1)
	{
		std::vector<std::string> recs = 
		{
			"\"1\",\"\",\"CANCEL\",\"2017-01-24T05:47:46\",\"U\",\"OC\",\"\",\"Y\",\"N\",\"ON\",\"2016-01-12\",\"\"",
			"\"3\",\"\",\"CANCEL\",\"2017-01-24T05:47:46\",\"C\",\"FC\",\"\",\"N\",\"Y\",\"\",\"\",\"2027-03-24\"",
			"\"4\",\"\",\"CANCEL\",\"2017-01-24T05:47:46\",\"C\",\"OC\",\"N\",\"Y\",\"Y\",\"OFF\",\"2017-04-24\",\"2027-04-24\"",
			"\"5\",\"\",\"CANCEL\",\"2017-01-24T05:47:46\",\"C\",\"UC\",\"N\",\"Y\",\"Y\",\"OFF\",\"\",\"\"",
			"\"2\",\"58919739\",\"NEW\",\"2017-01-24T05:47:46\",\"U\",\"\",\"Y\",\"Y\",\"Y\",\"ON\",\"2017-02-24\",\"\"",
		};

		for (auto it = recs.begin(); it != recs.end(); it++)
		{
			dtcc::database::tradeRecord emp;
			std::string::const_iterator iter = it->begin(), end = it->end(); 
			if (boost::spirit::qi::phrase_parse(iter, end, g, boost::spirit::ascii::space, emp) && iter == end)
			{
				std::cout << "DISSEMINATION_ID: " << emp.DISSEMINATION_ID << std::endl;
				std::cout << "ORIGINAL_DISSEMINATION_ID: " << emp.ORIGINAL_DISSEMINATION_ID << std::endl;
				std::cout << "ACTION: " << emp.ACTION << std::endl;
				std::cout << "EXECUTION_TIMESTAMP: " << emp.EXECUTION_TIMESTAMP << std::endl;
				std::cout << "CLEARED: " << emp.CLEARED << std::endl;
				std::cout << "INDICATION_OF_COLLATERALIZATION: " << emp.INDICATION_OF_COLLATERALIZATION << std::endl;
				if (!emp.INDICATION_OF_END_USER_EXCEPTION)
					std::cout << "INDICATION_OF_END_USER_EXCEPTION: (none)" << std::endl;
				else
					std::cout << "INDICATION_OF_END_USER_EXCEPTION: " << emp.INDICATION_OF_END_USER_EXCEPTION.get() << std::endl;

				std::cout << "INDICATION_OF_OTHER_PRICE_AFFECTING_TERM: " << emp.INDICATION_OF_OTHER_PRICE_AFFECTING_TERM << std::endl;
				std::cout << "BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS: " << emp.BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS << std::endl;
				
				if (!emp.EXECUTION_VENUE)
					std::cout << "EXECUTION_VENUE: (none)" << std::endl;
				else
					std::cout << "EXECUTION_VENUE: " << emp.EXECUTION_VENUE.get() << std::endl;

				std::cout << "-------------------------" << std::endl;
			}
			else
			{
				std::cout << "-------------------------\n";
				std::cout << "Parsing failed\n";
				std::cout << "-------------------------\n";
			}
		}
	}

	std::cout << "Conversion done in " << boost::chrono::duration_cast<boost::chrono::milliseconds> (
		boost::chrono::high_resolution_clock::now() - start) << std::endl;

	system("pause");
	return 0;
}

#else
#include <fstream>
#include <exception>
#include <array>

#include <boost/thread.hpp>

#include "application/curl/fileUrl.hpp"
#include "application/logger.hpp"
#include "application/service.hpp"
#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"

#include "database/connectors/sqlServer.hpp"

// temporary
struct configuration
{
	struct asset
	{
		// TODO: use the assetType factory
		dtcc::database::assetType type_;
		std::string fileStr_;
	};

	boost::gregorian::date start_;
	boost::gregorian::date end_;
	std::vector<asset> assets_;
	std::string baseUrl_;
	int bufferSize_;
};

// chrono
boost::chrono::high_resolution_clock::time_point start;

int main(int * argc, char ** argv)
{
	int ret = 1;

	try
	{
		// locale
		const std::locale format(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));
		dtcc::logger::initialize("dtccConsole_%Y%m%d.log", dtcc::severity::info);
		LOG_INFO() << "Application is starting";

		// create the db object
		LOG_INFO() << "Trying to connect to sql server";
		boost::shared_ptr<dtcc::database::connector> db(new dtcc::database::sqlServer());
		db->connect("dsn=sqlServer");
		dtcc::database::tradeRecordset rs(db);

		/*
		 * we load the settings here. For now we'll
		 * use only a few predefined settings but 
		 * we'll need to load a proper xml file 
		 */
		const configuration config =
		{
			boost::gregorian::from_simple_string("2017-01-10"),
			boost::gregorian::from_simple_string("2017-01-10"),
			{ configuration::asset { dtcc::database::assetType::commodity, "COMMODITIES" } },
			"https://kgc0418-tdw-data-0.s3.amazonaws.com/slices/",
			1000
		};

		auto dt = config.start_;

		// build the curl object	
		dtcc::curl * cnx = new dtcc::fileUrl();
		std::vector<dtcc::database::tradeRecord> recs;			// data buffer
		recs.reserve(config.bufferSize_);

		// main loop
		while (dt <= config.end_)
		{
			LOG_INFO() << "Start activity for " << boost::gregorian::to_simple_string(dt);

			for (auto It = config.assets_.cbegin(); It != config.assets_.cend(); It++)
			{
				std::stringstream fileName;
				fileName.imbue(format);
				fileName << "CUMULATIVE_" << It->fileStr_ << "_" << dt << ".zip";

				LOG_INFO()	<< "Loading " 
							<< It->fileStr_
							<< " data from URL: " 
							<< config.baseUrl_ + fileName.str();

				// we create an archive
				dtcc::archive<dtcc::zip::zip> ar(cnx->fetch(config.baseUrl_ + fileName.str(), 1024 * 1024));
				if (ar.open())
				{
					auto fs = ar.fileSystem();

					for (auto it = fs.begin(); it != fs.end(); it++)
					{
						std::stringstream file = ar.get(*it);

						LOG_INFO() << "Zip extraction successfull...";

						std::string linebuf; 
						std::getline(file, linebuf, '\n');								// trash the header

						start = boost::chrono::high_resolution_clock::now();
						int i = 0; while (std::getline(file, linebuf, '\n'))
						{	
							if (i >= config.bufferSize_)								// release the buffer toward the database
							{
								rs.insert(recs);
								recs.clear();
								i = 0;
							}

							recs.push_back(dtcc::database::tradeRecord(linebuf));
							i++; 
						}

						LOG_INFO()	<< "Conversion done in " 
									<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									   boost::chrono::high_resolution_clock::now() - start);
					}
				}
			}

			// add a day
			dt += boost::gregorian::date_duration(1);
		}

		LOG_INFO() << "Application is stopping";
		ret = 0;
	}
	catch (std::exception & e)
	{
		LOG_ERROR() << e.what();
		ret = 1;
	}

	system("pause");
	return ret;
}
#endif
#endif
