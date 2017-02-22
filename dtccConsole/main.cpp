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
#include <boost/tuple/tuple.hpp>

#include "database/recordsets/tradeRecordset.hpp"
#include "database/records/tradeRecordGrammar.hpp"

#define TESTT
#ifdef TESTT

boost::chrono::high_resolution_clock::time_point start;

const static std::string testMultiple(
R"s("58919739","","NEW","2017-01-10T07:52:46","U","FC","","N","N","OFF","2017-02-01","2017-02-28","","","CO","Energy","Commodity:Energy:Elec:Swap:Cash","Trade","ELECTRICITY-DAILY PHELIX BASE SPOT-EEX","","EUR:MWh","40.9775","","","MWH","MWH","3,400","3,400","","","","","","","","","","","","","","","",""
"58919740","","NEW","2017-01-10T07:52:46","U","FC","","N","N","ON","2017-02-01","2017-02-28","","","IR","Energy","Commodity:Energy:Elec:Swap:Cash","Trade","ELECTRICITY-DAILY PHELIX BASE SPOT-EEX","","EUR:MWh","40.9775","","","MWH","MWH","3,400","3,400","","","","","","","","","","","","","","","",""
"58919741","","NEW","2017-01-10T07:52:46","U","FC","","N","N","","2017-02-01","","","","EQ","Energy","Commodity:Energy:Elec:Swap:Cash","Trade","ELECTRICITY-DAILY PHELIX BASE SPOT-EEX","","EUR:MWh","40.9775","","","MWH","MWH","3,400","3,400","","","","","","","","","","","","","","","",""
)s");

int main(int * argc, char ** argv)
{
	bool print = true;
	std::vector<dtcc::database::tradeRecord> recs;

	start = boost::chrono::high_resolution_clock::now();

	std::string::const_iterator iter = testMultiple.begin(), end = testMultiple.end();

	if (dtcc::database::parse(iter, end, recs))
	{
		if (print)
		{
			for (auto it = recs.begin(); it != recs.end(); it++)
			{
				std::cout << "DISSEMINATION_ID: " << it->DISSEMINATION_ID << std::endl;
				std::cout << "ORIGINAL_DISSEMINATION_ID: " << it->ORIGINAL_DISSEMINATION_ID << std::endl;
				std::cout << "ACTION: " << it->ACTION << std::endl;
				std::cout << "EXECUTION_TIMESTAMP: " << it->EXECUTION_TIMESTAMP << std::endl;
				std::cout << "CLEARED: " << it->CLEARED << std::endl;
				std::cout << "INDICATION_OF_COLLATERALIZATION: " << it->INDICATION_OF_COLLATERALIZATION << std::endl;
				if (!it->INDICATION_OF_END_USER_EXCEPTION)
					std::cout << "INDICATION_OF_END_USER_EXCEPTION: (none)" << std::endl;
				else
					std::cout << "INDICATION_OF_END_USER_EXCEPTION: " << *it->INDICATION_OF_END_USER_EXCEPTION << std::endl;

				std::cout << "INDICATION_OF_OTHER_PRICE_AFFECTING_TERM: " << it->INDICATION_OF_OTHER_PRICE_AFFECTING_TERM << std::endl;
				std::cout << "BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS: " << it->BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS << std::endl;

				if (!it->EXECUTION_VENUE)
					std::cout << "EXECUTION_VENUE: (none)" << std::endl;
				else
					std::cout << "EXECUTION_VENUE: " << *it->EXECUTION_VENUE << std::endl;

				if (!it->EFFECTIVE_DATE)
					std::cout << "EFFECTIVE_DATE: (none)" << std::endl;
				else
					std::cout << "EFFECTIVE_DATE: " << *it->EFFECTIVE_DATE << std::endl;

				if (!it->END_DATE)
					std::cout << "END_DATE: (none)" << std::endl;
				else
					std::cout << "END_DATE: " << *it->END_DATE << std::endl;

				if (!it->SETTLEMENT_CURRENCY)
					std::cout << "SETTLEMENT_CURRENCY: (none)" << std::endl;
				else
					std::cout << "SETTLEMENT_CURRENCY: " << *it->SETTLEMENT_CURRENCY << std::endl;

				std::cout << "ASSET_TYPE: " << it->ASSET_CLASS << std::endl;

				if (it->SUBASSET_CLASS_FOR_OTHER_COMMODITY == "")
					std::cout << "SUBASSET_CLASS_FOR_OTHER_COMMODITY: (none)" << std::endl;
				else
					std::cout << "SUBASSET_CLASS_FOR_OTHER_COMMODITY: " << it->SUBASSET_CLASS_FOR_OTHER_COMMODITY << std::endl;

				if (it->TAXONOMY == "")
					std::cout << "TAXONOMY: (none)" << std::endl;
				else
					std::cout << "TAXONOMY: " << it->TAXONOMY << std::endl;

				std::cout << "PRICE_FORMING_CONTINUATION_DATA: " << it->PRICE_FORMING_CONTINUATION_DATA << std::endl;

				if (it->UNDERLYING_ASSET_1 == "")
					std::cout << "UNDERLYING_ASSET_1: (none)" << std::endl;
				else
					std::cout << "UNDERLYING_ASSET_1: " << it->UNDERLYING_ASSET_1 << std::endl;

				if (it->UNDERLYING_ASSET_2 == "")
					std::cout << "UNDERLYING_ASSET_2: (none)" << std::endl;
				else
					std::cout << "UNDERLYING_ASSET_2: " << it->UNDERLYING_ASSET_2 << std::endl;

				if (it->PRICE_NOTATION_TYPE == "")
					std::cout << "PRICE_NOTATION_TYPE: (none)" << std::endl;
				else
					std::cout << "PRICE_NOTATION_TYPE: " << it->PRICE_NOTATION_TYPE << std::endl;

				if (!it->PRICE_NOTATION)
					std::cout << "PRICE_NOTATION: (none)" << std::endl;
				else
					std::cout << "PRICE_NOTATION: " << std::fixed << std::setprecision(6) << it->PRICE_NOTATION << std::endl;

				if (it->ADDITIONAL_PRICE_NOTATION_TYPE == "")
					std::cout << "ADDITIONAL_PRICE_NOTATION_TYPE: (none)" << std::endl;
				else
					std::cout << "ADDITIONAL_PRICE_NOTATION_TYPE: " << it->ADDITIONAL_PRICE_NOTATION_TYPE << std::endl;

				if (!it->ADDITIONAL_PRICE_NOTATION)
					std::cout << "ADDITIONAL_PRICE_NOTATION: (none)" << std::endl;
				else
					std::cout << "ADDITIONAL_PRICE_NOTATION: " << std::fixed << std::setprecision(6) << it->ADDITIONAL_PRICE_NOTATION << std::endl;

				if (it->NOTIONAL_CURRENCY_1 == "")
					std::cout << "NOTIONAL_CURRENCY_1: (none)" << std::endl;
				else
					std::cout << "NOTIONAL_CURRENCY_1: " << it->NOTIONAL_CURRENCY_1 << std::endl;

				if (it->NOTIONAL_CURRENCY_2 == "")
					std::cout << "NOTIONAL_CURRENCY_2: (none)" << std::endl;
				else
					std::cout << "NOTIONAL_CURRENCY_2: " << it->NOTIONAL_CURRENCY_2 << std::endl;

				if (!it->ROUNDED_NOTIONAL_AMOUNT_1)
					std::cout << "ROUNDED_NOTIONAL_AMOUNT_1: (none)" << std::endl;
				else
				{
					std::cout << "ROUNDED_NOTIONAL_AMOUNT_1: "
						<< boost::get<0>(*it->ROUNDED_NOTIONAL_AMOUNT_1);

					if (boost::get<1>(*it->ROUNDED_NOTIONAL_AMOUNT_1) == true)
					{
						std::cout << " (Plus)" << std::endl;
					}
					else std::cout << std::endl;
				}

				if (!it->ROUNDED_NOTIONAL_AMOUNT_2)
					std::cout << "ROUNDED_NOTIONAL_AMOUNT_2: (none)" << std::endl;
				else
				{
					std::cout << "ROUNDED_NOTIONAL_AMOUNT_2: "
						<< boost::get<0>(*it->ROUNDED_NOTIONAL_AMOUNT_2);

					if (boost::get<1>(*it->ROUNDED_NOTIONAL_AMOUNT_2) == true)
					{
						std::cout << " (Plus)" << std::endl;
					}
					else std::cout << std::endl;
				}

				if (it->PAYMENT_FREQUENCY_1 == "")
					std::cout << "PAYMENT_FREQUENCY_1: (none)" << std::endl;
				else
					std::cout << "PAYMENT_FREQUENCY_1: " << it->PAYMENT_FREQUENCY_1 << std::endl;

				if (it->PAYMENT_FREQUENCY_2 == "")
					std::cout << "PAYMENT_FREQUENCY_2: (none)" << std::endl;
				else
					std::cout << "PAYMENT_FREQUENCY_2: " << it->PAYMENT_FREQUENCY_2 << std::endl;

				if (it->RESET_FREQUENCY_1 == "")
					std::cout << "RESET_FREQUENCY_1: (none)" << std::endl;
				else
					std::cout << "RESET_FREQUENCY_1: " << it->RESET_FREQUENCY_1 << std::endl;

				if (it->RESET_FREQUENCY_2 == "")
					std::cout << "RESET_FREQUENCY_2: (none)" << std::endl;
				else
					std::cout << "RESET_FREQUENCY_2: " << it->RESET_FREQUENCY_2 << std::endl;

				if (it->EMBEDED_OPTION)
					std::cout << "EMBEDED_OPTION: true" << std::endl;
				else
					std::cout << "EMBEDED_OPTION: false" << std::endl;

				if (!it->OPTION_STRIKE_PRICE)
					std::cout << "OPTION_STRIKE_PRICE: (none)" << std::endl;
				else
					std::cout << "OPTION_STRIKE_PRICE: " << it->OPTION_STRIKE_PRICE << std::endl;

				if (!it->OPTION_TYPE)
					std::cout << "OPTION_TYPE: (none)" << std::endl;
				else
					std::cout << "OPTION_TYPE: " << *it->OPTION_TYPE << std::endl;

				if (!it->OPTION_FAMILY)
					std::cout << "OPTION_FAMILY: (none)" << std::endl;
				else
					std::cout << "OPTION_FAMILY: " << *it->OPTION_FAMILY << std::endl;

				std::cout << "-------------------------" << std::endl;
			}
		}

		std::cout << recs.size() << " conversions done in " << boost::chrono::duration_cast<boost::chrono::milliseconds> (
			boost::chrono::high_resolution_clock::now() - start) << std::endl;
	}
	else
	{
		std::cout << "conversion failed" << std::endl;
	}

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
