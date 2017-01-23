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
#include <boost/date_time/posix_time/posix_time.hpp>

#include "application/curl/fileUrl.hpp"
#include "application/logger.hpp"
#include "application/service.hpp"
#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"

struct asset
{
	enum type
	{
		rate = 1,
		commodity = 2,
		equity = 3,
		currency = 4,
		credit = 5
	};

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
class dbRecord
{
public:

	enum class action
	{
		new_ = 1,
		cancel_ = 2,
		correct_ = 3
	};

	enum class collateralization
	{
		fc = 1,
		oc = 2,
		pc = 3,
		uc = 4
	};

	dbRecord() {};
	dbRecord(const std::string & line)
	{
		std::string token;
		std::stringstream ss(line);

		std::getline(ss, token, ',');
		DISSEMINATION_ID = boost::lexical_cast<int64_t>(token.substr(1, token.size() - 2));

		std::getline(ss, token, ',');
		if (token != "\"\"")
			ORIGINAL_DISSEMINATION_ID = boost::lexical_cast<int64_t>(token.substr(1, token.size() - 2));
		else
			ORIGINAL_DISSEMINATION_ID = boost::none;
	}
private:
	int64_t DISSEMINATION_ID;
	boost::optional<int64_t> ORIGINAL_DISSEMINATION_ID;
	action ACTION;
	boost::posix_time::ptime EXECUTION_TIMESTAMP;
	bool CLEARED;
	boost::optional<int64_t> INDICATION_OF_COLLATERALIZATION;
	boost::optional<bool> INDICATION_OF_END_USER_EXCEPTION;
	bool INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
	bool BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;
	bool EXECUTION_VENUE;
	boost::gregorian::date EFFECTIVE_DATE;
	boost::gregorian::date END_DATE;
	std::string DAY_COUNT_CONVENTION;
	char SETTLEMENT_CURRENCY[3];
	asset::type ASSET_CLASS;
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

		size_t buffSize = 1000;
		std::vector<dbRecord> recs;			// data buffer
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

							recs.push_back(dbRecord(linebuf));
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