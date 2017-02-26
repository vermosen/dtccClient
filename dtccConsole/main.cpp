#ifndef MAIN_CPP_
#define MAIN_CPP_

#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define FUSION_MAX_VECTOR_SIZE 45

#include <fstream>
#include <exception>
#include <array>
#include <vector>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "application/curl/fileUrl.hpp"
#include "application/logger.hpp"
#include "application/service.hpp"
#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"

#include "database/recordsets/tradeRecordset.hpp"
#include "database/connectors/sqlServer.hpp"
#include "database/parse.hpp"

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
						std::string file = ar.get(*it).str();

						LOG_INFO() << "Zip extraction successfull...";

						start = boost::chrono::high_resolution_clock::now();
						
						LOG_INFO() << "Starting record conversion...";

						std::vector<dtcc::database::tradeRecord> recs; recs.reserve(50000);

						std::string::const_iterator iter = file.begin(), end = file.end();

						// skip the header
						while (*iter++ != '\n');

						if (dtcc::database::parse(iter, end, recs, dt))
						{
							LOG_INFO() << recs.size() << "conversion done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start);

							// if the buffer reached the max size, writing in the buffer in the database
							if (recs.size() > config.bufferSize_)
							{
								rs.insert(recs);
								recs.clear();

								LOG_INFO() << recs.size() << "insertion done in "
									<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
										boost::chrono::high_resolution_clock::now() - start);
							}
						}
						else
						{
							LOG_ERROR() << "An error has occurred while converting the records";
						}
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
