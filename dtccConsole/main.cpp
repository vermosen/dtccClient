#ifndef MAIN_CPP_
#define MAIN_CPP_

#define FUSION_MAX_VECTOR_SIZE 45

#include <fstream>
#include <streambuf>
#include <exception>
#include <array>
#include <vector>
#include <functional>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"
#include "application/webConnectors/all.hpp"

#include "database/recordsets/tradeRecordset.hpp"
#include "database/connectors/sqlServer.hpp"
#include "database/record/parser/parseRecords.hpp"
#include "application/logger.hpp"
#include "application/startup.hpp"
#include "application/queries/eod.hpp"

#include "settings/parser/parseSettings.hpp"
#include "settings.hpp"

#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif

// chrono
boost::chrono::high_resolution_clock::time_point start;

int main(int argc, char ** argv)
{
	int ret = 1;

	try
	{
		auto args = dtcc::parseStartup(argc, argv);
		
		if (args.find("settings") == args.cend())
		{
			std::cerr << "setting file path cannot be found";
			return 1;
		}

		std::ifstream file(args["settings"], std::ios::in | std::ios::binary);
		std::stringstream buffer; std::string raw; dtcc::settings settings;

		if (file.is_open())
		{
			buffer << file.rdbuf();
			raw = buffer.str();
			
			if (!dtcc::parser::parseSettings(raw.cbegin(), raw.cend(), settings))
			{
				std::cerr << "failed to decode settings";
				return 1;
			}
		}

		// create the logger
		dtcc::logger::initialize(settings.logger_.fileStr_, settings.logger_.severity_);
		LOG_INFO() << "Application is starting...";

		// create the db object
		LOG_INFO() << "Trying to connect to sql server";
		boost::shared_ptr<dtcc::database::connector> db(new dtcc::database::sqlServer());	// TODO: get the db type from the settings
		db->connect(settings.database_);
		dtcc::database::tradeRecordset rs(db);

		// build the webConnector object	
		auto cnx = dtcc::abstractFactory<dtcc::webConnector, std::string, dtcc::webConnector::args>::createInstance(
			settings.connector_.type_, 
			dtcc::webConnector::args(
				settings.connector_.protocol_, 
				settings.connector_.host_, 
				settings.connector_.port_));

		std::vector<dtcc::database::tradeRecord> recs;			// data buffer
		recs.reserve(settings.memory_ / sizeof(dtcc::database::tradeRecord));

		// main loop
		for (auto dt = settings.startDate_; dt <= settings.endDate_; dt += boost::gregorian::date_duration(1))
		{
			LOG_INFO() << "Start activity on " << boost::gregorian::to_simple_string(dt);

			for (auto it = settings.assets_.cbegin(); it != settings.assets_.cend(); it++)
			{
				dtcc::eod q(dt, *it);

				// we create an archive
				dtcc::archive<dtcc::zip::zip> ar(std::move(*cnx->fetch(q)));
				
				if (!ar.open())
				{
					LOG_ERROR() << "failed to open the archive ";
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

						if (dtcc::parser::parseRecords(file.begin(), file.end(), recs, dt))
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
