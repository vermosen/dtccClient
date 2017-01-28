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

#include <boost/thread.hpp>


#include "application/curl/fileUrl.hpp"
#include "application/logger.hpp"
#include "application/service.hpp"
#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"

#include "database/record.hpp"

struct asset
{
	// TODO: use the assetType factory
	dtcc::database::assetType type_;
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
			{ asset{ dtcc::database::assetType::commodity, "COMMODITIES" } },
			"https://kgc0418-tdw-data-0.s3.amazonaws.com/slices/"
		};

		const std::locale format(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));

		auto dt = config.start_;

		// build the curl object	
		dtcc::curl * cnx = new dtcc::fileUrl();

		size_t buffSize = 1000;
		std::vector<dtcc::database::record> recs;			// data buffer
		recs.reserve(buffSize);

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
						std::getline(file, linebuf, '\n');			// trash the header
						int i = 0; while (std::getline(file, linebuf, '\n'))
						{	
							if (i == buffSize)
							{
								// release the buffer toward the database
								recs.clear();
								i = 0;
							}

							recs.push_back(dtcc::database::record(linebuf));
							i++; 
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