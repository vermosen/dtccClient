#include <boost/algorithm/string/split.hpp>
#include <boost/locale.hpp>

#include "database/records/tradeRecord.hpp"
#include "pattern/abstractFactory.hpp"

namespace dtcc
{
	namespace database
	{
		inline double convert(const std::string & val)
		{
			std::istringstream buffer(val);
			buffer.imbue(std::locale(""));
			double res;
			buffer >> res;
			return res;
		};

		tradeRecord::tradeRecord(const std::string & line)
		{
			// split using "," token
			std::vector<std::string> words;

			boost::iter_split(words, line, boost::algorithm::first_finder("\",\""));

			DISSEMINATION_ID = boost::lexical_cast<int64_t>(words[0].substr(1, words[0].size() - 2));

			if (words[1] != "")
				ORIGINAL_DISSEMINATION_ID = boost::lexical_cast<int64_t>(words[1]);
			else
				ORIGINAL_DISSEMINATION_ID = boost::none;

			ACTION = EnumManager<action>::toEnum(words[2]);

			EXECUTION_TIMESTAMP = boost::date_time::parse_delimited_time<boost::posix_time::ptime>(
				words[3], 'T');

			if (words[4] == "C") CLEARED = true;

			if (words[5] != "")
				INDICATION_OF_COLLATERALIZATION = EnumManager<collateralization>::toEnum(words[5]);
			else
				INDICATION_OF_COLLATERALIZATION = boost::none;

			if (words[6] == "")
				INDICATION_OF_END_USER_EXCEPTION = boost::none;
			else if (words[6] == "Y")
				INDICATION_OF_END_USER_EXCEPTION = true;
			else
				INDICATION_OF_END_USER_EXCEPTION = false;

			if (words[7] == "Y")
				INDICATION_OF_OTHER_PRICE_AFFECTING_TERM = true;
			else
				INDICATION_OF_OTHER_PRICE_AFFECTING_TERM = false;

			if (words[8] == "Y")
				BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS = true;
			else
				BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS = false;

			if (words[9] == "")
				EXECUTION_VENUE = boost::none;
			else if (words[9] == "ON")
				EXECUTION_VENUE = true;
			else
				EXECUTION_VENUE = false;

			if (words[10] == "")
				EFFECTIVE_DATE = boost::none;
			else
				EFFECTIVE_DATE = boost::gregorian::from_string(words[10]);
			
			if (words[11] == "")
				END_DATE = boost::none;
			else
				END_DATE = boost::gregorian::from_string(words[11]);

			DAY_COUNT_CONVENTION = words[12];

			if (words[13] == "")
				SETTLEMENT_CURRENCY = boost::none;
			else
				SETTLEMENT_CURRENCY = std::array<char, 3> { words[13][0], words[13][1], words[13][2] };

			ASSET_CLASS = EnumManager<assetType>::toEnum(words[14]);

			SUBASSET_CLASS_FOR_OTHER_COMMODITY = words[15];
			TAXONOMY = words[16];

			PRICE_FORMING_CONTINUATION_DATA = EnumManager<priceFormingContinuationData>::toEnum(words[17]);
		
			UNDERLYING_ASSET_1 = words[18];
			UNDERLYING_ASSET_2 = words[19];
			PRICE_NOTATION_TYPE = words[20];

			if (words[21] == "")
				PRICE_NOTATION = boost::none;
			else
				PRICE_NOTATION = convert(words[21]);

			ADDITIONAL_PRICE_NOTATION_TYPE = words[22];
			ADDITIONAL_PRICE_NOTATION = words[23];
			NOTIONAL_CURRENCY_1 = words[24];
			NOTIONAL_CURRENCY_2 = words[25];
			
			if (words[26] == "")
				ROUNDED_NOTIONAL_AMOUNT_1 = boost::none;
			else
			{
				if (*words[26].rbegin() == '+')
				{
					ROUNDED_NOTIONAL_AMOUNT_1 = convert(words[26].substr(0, words[26].size() - 2));
					ROUNDED_NOTIONAL_AMOUNT_1_PLUS = true;
				}
				else
				{
					ROUNDED_NOTIONAL_AMOUNT_1 = convert(words[26].substr(0, words[26].size() - 1));
				}
			}

			if (words[27] == "")
				ROUNDED_NOTIONAL_AMOUNT_2 = boost::none;
			else
			{
				if (*words[27].rbegin() == '+')
				{
					ROUNDED_NOTIONAL_AMOUNT_2 = convert(words[27].substr(0, words[27].size() - 2));
					ROUNDED_NOTIONAL_AMOUNT_2_PLUS = true;
				}
				else
				{
					ROUNDED_NOTIONAL_AMOUNT_2 = convert(words[27].substr(0, words[27].size() - 1));
				}
			}

			PAYMENT_FREQUENCY_1 = words[28];
			PAYMENT_FREQUENCY_2 = words[29];
			RESET_FREQUENCY_1 = words[30];
			RESET_FREQUENCY_2 = words[31];

			if (words[32] == "EMBED1") EMBEDED_OPTION = true;

			if (words[33] == "")
				OPTION_STRIKE_PRICE = boost::none;
			else
				OPTION_STRIKE_PRICE = convert(words[33]);
			
			if (words[34] == "")
				OPTION_TYPE = boost::none;
			else
				OPTION_TYPE = EnumManager<optionType>::toEnum(words[34]);
			
			if (words[35] == "")
				OPTION_FAMILY = boost::none;
			else
				OPTION_FAMILY = EnumManager<optionFamily>::toEnum(words[35]);

			if (words[36] == "")
				OPTION_CURRENCY = boost::none;
			else
				OPTION_CURRENCY = std::array<char, 3> { words[36][0], words[36][1], words[36][2] };

			if (words[37] == "")
				OPTION_PREMIUM = boost::none;
			else
				OPTION_PREMIUM = convert(words[37]);

			if (words[38] == "")
				OPTION_LOCK_PERIOD = boost::none;
			else
				OPTION_LOCK_PERIOD = boost::gregorian::from_string(words[38]);

			if (words[39] == "")
				OPTION_EXPIRATION_DATE = boost::none;
			else
				OPTION_EXPIRATION_DATE = boost::gregorian::from_string(words[39]);

			PRICE_NOTATION_TYPE_2 = words[40];

			if (words[41] == "")
				PRICE_NOTATION_2 = boost::none;
			else
				PRICE_NOTATION_2 = convert(words[41]);

			PRICE_NOTATION_TYPE_3 = words[42];

			if (words[43] == "")
				PRICE_NOTATION_3 = boost::none;
			else
				PRICE_NOTATION_3 = convert(words[43].substr(0, words[43].size() - 2));
		}
	}
}