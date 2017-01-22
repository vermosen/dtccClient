#ifndef MAIN_CPP_
#define MAIN_CPP_

#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <array>
#include <fstream>
#include <exception>

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "application/curl/fileUrl.hpp"
#include "application/logger.hpp"
#include "application/service.hpp"
#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"

struct asset
{
	std::string ticker_;
	std::string fileStr_;
};

// temporary
struct configuration
{
	boost::gregorian::date start_;
	boost::gregorian::date end_;
	std::vector<asset> assets_;
	std::string baseUrl_;
};

// the record ready for db insertion
struct dbRecord
{
	int64_t DISSEMINATION_ID;
	boost::optional<int64_t> ORIGINAL_DISSEMINATION_ID;

};

int main(int * argc, char ** argv)
{
	int ret = 1;

	try
	{
		dtcc::logger::setLogger("dtccConsole_%Y%m%d.log", dtcc::logger::Info);
		LOG_INFO() << "Application is starting";
		LOG_INFO() << "Trying to create a new Windows service";

		/*
		 * we load the settings here. For now we'll
		 * use only a few predefined settings but 
		 * we'll need to load a proper xml file 
		 */
		const configuration config =
		{
			boost::gregorian::from_simple_string("2017-01-10"),
			boost::gregorian::from_simple_string("2017-01-10"),
			{ asset{ "CO", "COMMODITIES" } },
			"https://kgc0418-tdw-data-0.s3.amazonaws.com/slices/"
		};

		const std::locale format(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));

		auto dt = config.start_;

		// build the curl object	
		dtcc::curl * cnx = new dtcc::fileUrl();

		std::array<dbRecord, 100> recs; // data buffer

		auto ptr = recs.begin();
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
							<< It->ticker_ 
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
						std::string line; 
						std::getline(file, line, '\n');			// trash the header
						while (std::getline(file, line, '\n'))
						{	
							if (ptr == recs.end())
							{
								// release the buffer toward the database
								ptr = recs.begin();
							}

							std::string token;
							std::stringstream ss(line);

							std::getline(ss, token, ',');
							ptr->DISSEMINATION_ID = boost::lexical_cast<int64_t>(token.substr(1, token.size() - 2));

							std::getline(ss, token, ',');
							if (token != "\"\"")
								ptr->ORIGINAL_DISSEMINATION_ID = boost::lexical_cast<int64_t>(token.substr(1, token.size() - 2));
							else
								ptr->ORIGINAL_DISSEMINATION_ID = boost::none;

							ptr++; 
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

/*
////// begin test
std::stringstream sOut;
std::stringstream sIn("hello world");

{
boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sOut);
boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
in.push(boost::iostreams::zlib_compressor());
in.push(sIn);
boost::iostreams::copy(in, out);
}

sIn.clear(); sIn.seekp(0, std::ios::beg);
sOut.seekg(0, std::ios::beg);

{
boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sIn);
boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
in.push(boost::iostreams::zlib_decompressor());
in.push(sOut);
boost::iostreams::copy(in, out);
}

std::string res = sIn.str();
////// end test
*/