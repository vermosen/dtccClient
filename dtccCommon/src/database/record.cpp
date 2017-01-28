#include "database/record.hpp"
#include "pattern/abstractFactory.hpp"

namespace dtcc
{
	namespace database
	{
		record::record(const std::string & line)
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

			std::getline(ss, token, ',');
			ACTION = EnumManager<action>::toEnum(token.substr(1, token.size() - 2));

			std::getline(ss, token, ',');
			EXECUTION_TIMESTAMP = boost::date_time::parse_delimited_time<boost::posix_time::ptime>(
				token.substr(1, token.size() - 2), 'T');

			std::getline(ss, token, ',');
			if (token == "\"C\"") CLEARED = true;

			std::getline(ss, token, ',');
			if (token != "\"\"")
				INDICATION_OF_COLLATERALIZATION = EnumManager<collateralization>::toEnum(token.substr(1, token.size() - 2));
			else
				INDICATION_OF_COLLATERALIZATION = boost::none;

			std::getline(ss, token, ',');
			if (token == "\"\"")
				INDICATION_OF_END_USER_EXCEPTION = boost::none;
			else if (token == "\"Y\"")
				INDICATION_OF_END_USER_EXCEPTION = true;
			else
				INDICATION_OF_END_USER_EXCEPTION = false;

			std::getline(ss, token, ',');
			if (token == "\"Y\"")
				INDICATION_OF_OTHER_PRICE_AFFECTING_TERM = true;
			else
				INDICATION_OF_OTHER_PRICE_AFFECTING_TERM = false;

			std::getline(ss, token, ',');
			if (token == "\"Y\"")
				BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS = true;
			else
				BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS = false;

			std::getline(ss, token, ',');
			if (token == "\"\"")
				EXECUTION_VENUE = boost::none;
			else if (token == "\"ON\"")
				EXECUTION_VENUE = true;
			else
				EXECUTION_VENUE = false;

			std::getline(ss, token, ',');
			if (token == "\"\"")
				EFFECTIVE_DATE = boost::none;
			else
				EFFECTIVE_DATE = boost::gregorian::from_string(token.substr(1, token.size() - 2));
			
			std::getline(ss, token, ',');
			if (token == "\"\"") 
				END_DATE = boost::none;
			else
				END_DATE = boost::gregorian::from_string(token.substr(1, token.size() - 2));

			std::getline(ss, token, ',');
			DAY_COUNT_CONVENTION = token.substr(1, token.size() - 2);

			std::getline(ss, token, ',');
			if (token == "\"\"")
				SETTLEMENT_CURRENCY = boost::none;
			else
			{
				auto temp = token.substr(1, token.size() - 2);
				std::copy(temp.begin(), temp.end(), SETTLEMENT_CURRENCY.begin());
			}
		}
	}
}