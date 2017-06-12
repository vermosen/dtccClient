#include "parsing.hpp"

#include <fstream>

#include <boost/chrono.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "database/record/parser/tradeRecordGrammar.hpp"
#include "database/record/parser/parseRecords.hpp"
#include "database/record/enum.hpp"

#include "settings/parser/parseSettings.hpp"

boost::chrono::high_resolution_clock::time_point start;

static std::string testMultiple(
	R"s("58919739","","NEW","2017-01-10T07:52:46","U","FC","","N","N","OFF","2017-02-01","2017-02-28","","","CO","Energy","Commodity:Energy:Elec:Swap:Cash","Trade","ELECTRICITY-DAILY PHELIX BASE SPOT-EEX","","EUR:MWh","40.9775","","","MWH","MWH","3,400","3,400","","","","","","","","","","","","","","","",""
"58919722","","NEW","2017-01-10T07:52:45","U","FC","","N","N","OFF","2017-02-01","2017-02-28","","","CO","Energy","Commodity:Energy:Elec:Swap:Cash","Trade","ELECTRICITY-DAILY PHELIX BASE SPOT-EEX","","EUR:MWh","40.9775","","","MWH","MWH","3,400","3,400","","","","","","","","","","","","","","","",""
"58919741","","NEW","2017-01-10T07:52:46","U","FC","","N","N","","2017-02-01","","","","EQ","Energy","Commodity:Energy:Elec:Swap:Cash","Trade","ELECTRICITY-DAILY PHELIX BASE SPOT-EEX","","EUR:MWh","40.9775","","","MWH","MWH","3,400","3,400","","","","","","","","","","","","","","","",""
)s");

namespace testSuite
{
	void parsing::multipleRecords()
	{
		std::vector<dtcc::database::tradeRecord> recs;

		start = boost::chrono::high_resolution_clock::now();

		std::string::iterator iter = testMultiple.begin(), end = testMultiple.end();

		boost::gregorian::date dt(2012, 01, 01);

		if (dtcc::parser::parseRecords(iter, end, recs, dt))
		{
			bool print = false;

			if (print)
			{
				for (auto it = recs.begin(); it != recs.end(); it++)
				{
					std::cout << "DISSEMINATION_ID: " << it->DISSEMINATION_ID << std::endl;
					std::cout << "ORIGINAL_DISSEMINATION_ID: " << it->ORIGINAL_DISSEMINATION_ID << std::endl;
					std::cout << "ACTION: " << it->ACTION << std::endl;
					std::cout << "EXECUTION_TIMESTAMP: " << it->EXECUTION_TIMESTAMP << std::endl;
					std::cout << "CLEARED: " << it->CLEARED << std::endl;

					if (!it->INDICATION_OF_COLLATERALIZATION)
						std::cout << "INDICATION_OF_COLLATERALIZATION: (none)" << std::endl;
					else
						std::cout << "INDICATION_OF_COLLATERALIZATION: " << *it->INDICATION_OF_COLLATERALIZATION << std::endl;

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

			BOOST_TEST(true);
		}
		else
		{
			BOOST_ERROR("\n" << " parsing failed");
		}
	}

	void parsing::preference()
	{
		std::ifstream file("C:\\Git\\dtccClient\\testSuite\\settings\\dev.xml", std::ios::in | std::ios::binary);
		std::stringstream buffer; std::string raw; dtcc::settings settings;

		if (file.is_open())
		{
			buffer << file.rdbuf();
			raw = buffer.str();

			if (!dtcc::parser::parseSettings(raw.cbegin(), raw.cend(), settings))
			{
				BOOST_ERROR("\n" << " parsing failed");
			}
			else
			{
				BOOST_TEST(true);
			}
		}
	}

	boost::unit_test_framework::test_suite * parsing::suite()
	{
		boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("parsing Tests");
		suite->add(BOOST_TEST_CASE(&parsing::multipleRecords));
		suite->add(BOOST_TEST_CASE(&parsing::preference));
		return suite;
	}
}



