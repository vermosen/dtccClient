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
#include <streambuf>
#include <exception>
#include <array>
#include <vector>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"
#include "application/curl/fileUrl.hpp"
#include "application/logger.hpp"

#include "database/recordsets/tradeRecordset.hpp"
#include "database/connectors/sqlServer.hpp"
#include "record/parser/parseRecords.hpp"
#include "application/startup.hpp"
//#include "application/settings/parser/parseSettings.hpp"
#include "application/settings.hpp"

// chrono
boost::chrono::high_resolution_clock::time_point start;

std::map<std::string, std::string> readArgs(int argc, char ** argv)
{
	std::map<std::string, std::string> ret;

	for (int i = 1; i < argc; i++)
	{
		std::string s = std::string(argv[i]);

		auto pos = s.find('=');

		if (pos != std::string::npos && s.substr(pos + 1).find('=') == std::string::npos)
		{
			ret.insert(std::make_pair(s.substr(1, pos - 1), s.substr(pos + 1)));
		}
	}

	return ret;
}

int main(int argc, char ** argv)
{
	int ret = 1;

	try
	{
		auto args = dtcc::parseStartup(argc, argv);
		
		if (args.find("settings") == args.cend())
		{
			LOG_FATAL() << "setting file path cannot be found";
			return 1;
		}

		std::ifstream file(args["settings"], std::ios::in | std::ios::binary);
		std::stringstream buffer; std::string raw; dtcc::settings2 settings;

		if (file.is_open())
		{
			buffer << file.rdbuf();
			raw = buffer.str();
			/*
			if (!dtcc::parser::parseSettings(raw.cbegin(), raw.cend(), settings))
			{
				LOG_FATAL() << "failed to decode settings";
				return 1;
			}*/
		}

		// create the logger
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
		const dtcc::settings config =
		{
			boost::gregorian::from_simple_string("2016-11-01"), // start 2016-08-11
			boost::gregorian::from_simple_string("2017-01-31"),
			{
				dtcc::settings::asset{ dtcc::database::assetType::interestRate	, "RATES"		},
				dtcc::settings::asset{ dtcc::database::assetType::currency		, "FOREX"		},
				dtcc::settings::asset{ dtcc::database::assetType::commodity		, "COMMODITIES" },
				dtcc::settings::asset{ dtcc::database::assetType::credit		, "CREDITS"		},
				dtcc::settings::asset{ dtcc::database::assetType::equity		, "EQUITIES"	}
			},
			"https://kgc0418-tdw-data-0.s3.amazonaws.com/slices/",
			30 * 1024 * 1024
		};

		auto dt = config.start_;

		// formatter
		auto * formatter = new boost::gregorian::date_facet("%Y_%m_%d");
		const std::locale format(std::locale::classic(), formatter);

		// build the curl object	
		dtcc::curl * cnx = new dtcc::fileUrl();
		std::vector<dtcc::database::tradeRecord> recs;			// data buffer
		recs.reserve(config.memory_ / sizeof(dtcc::database::tradeRecord));

		// main loop
		while (dt <= config.end_)
		{
			LOG_INFO() << "Start activity for " << boost::gregorian::to_simple_string(dt);

			for (auto it = config.assets_.cbegin(); it != config.assets_.cend(); it++)
			{
				std::stringstream fileName;
				fileName.imbue(format);
				fileName << "CUMULATIVE_" << it->fileStr_ << "_" << dt << ".zip";

				LOG_INFO()	<< "Loading " 
							<< it->fileStr_
							<< " data from URL: " 
							<< config.baseUrl_ + fileName.str();

				// we create an archive
				dtcc::archive<dtcc::zip::zip> ar(cnx->fetch(config.baseUrl_ + fileName.str(), 1024 * 1024));
				
				if (!ar.open())
				{
					LOG_ERROR() << "failed to open the archive";
				}
				else
				{
					auto fs = ar.fileSystem();

					for (auto jt = fs.begin(); jt != fs.end(); jt++)
					{
						std::string file = ar.get(*jt).str();

						LOG_INFO() << "Zip extraction successfull...";

						start = boost::chrono::high_resolution_clock::now();

						LOG_INFO() << "Starting record conversion...";

						std::string::const_iterator iter = file.begin(), end = file.end();

						if (dtcc::parser::parseRecords(iter, end, recs, dt))
						{
							LOG_INFO() << recs.size() << " conversions done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start);

							start = boost::chrono::high_resolution_clock::now();

							// date facet
							std::ostringstream os;
							auto * temp(new boost::gregorian::date_facet("%Y-%m-%d"));
							os.imbue(std::locale(os.getloc(), temp));
							os << dt;

							std::string id = boost::lexical_cast<std::string>(static_cast<int>(it->type_));
							std::string filter = "FILE_DATE = '" + os.str() + "' AND ASSET_CLASS = " + id;

							rs.remove(filter); 
							
							start = boost::chrono::high_resolution_clock::now();

							LOG_INFO() << "Trades cleanup done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start);

							start = boost::chrono::high_resolution_clock::now();

							auto ids = rs.insert(recs); 
							recs.clear();

							LOG_INFO() << ids.size() << " insertions done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start);
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
