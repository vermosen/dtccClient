 /*
* aletheiaDatabase.hpp
*
*  Created on: Jul 3, 2016
*      Author: vermosen
*/

#ifndef CASSANDRA_RECORDSETS_TRADE_HPP_
#define CASSANDRA_RECORDSETS_TRADE_HPP_

#include <cassandra.h>

#include "database/recordset.hpp"
#include "database/cassandra/connector.hpp"
#include "database/record/tradeRecord.hpp"

#define FIELDS	"DISSEMINATION_ID, " \
				"ORIGINAL_DISSEMINATION_ID, " \
				"ACTION, " \
				"EXECUTION_TIMESTAMP, " \
				"CLEARED, " \
				"INDICATION_OF_COLLATERALIZATION, " \
				"INDICATION_OF_END_USER_EXCEPTION, " \
				"INDICATION_OF_OTHER_PRICE_AFFECTING_TERM, " \
				"BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS, " \
				"EXECUTION_VENUE, " \
				"EFFECTIVE_DATE, " \
				"END_DATE, " \
				"DAY_COUNT_CONVENTION, " \
				"SETTLEMENT_CURRENCY, " \
				"ASSET_CLASS, " \
				"SUBASSET_CLASS_FOR_OTHER_COMMODITY, " \
				"TAXONOMY, " \
				"PRICE_FORMING_CONTINUATION_DATA, " \
				"UNDERLYING_ASSET_1, " \
				"UNDERLYING_ASSET_2, " \
				"PRICE_NOTATION_TYPE, " \
				"PRICE_NOTATION, " \
				"ADDITIONAL_PRICE_NOTATION_TYPE, " \
				"ADDITIONAL_PRICE_NOTATION, " \
				"NOTIONAL_CURRENCY_1, " \
				"NOTIONAL_CURRENCY_2, " \
				"ROUNDED_NOTIONAL_AMOUNT_1, " \
				"ROUNDED_NOTIONAL_AMOUNT_1_PLUS, " \
				"ROUNDED_NOTIONAL_AMOUNT_2, " \
				"ROUNDED_NOTIONAL_AMOUNT_2_PLUS, " \
				"PAYMENT_FREQUENCY_1, " \
				"PAYMENT_FREQUENCY_2, " \
				"RESET_FREQUENCY_1, " \
				"RESET_FREQUENCY_2, " \
				"EMBEDED_OPTION, " \
				"OPTION_STRIKE_PRICE, " \
				"OPTION_TYPE, " \
				"OPTION_FAMILY, " \
				"OPTION_CURRENCY, " \
				"OPTION_PREMIUM, " \
				"OPTION_LOCK_PERIOD, " \
				"OPTION_EXPIRATION_DATE, " \
				"PRICE_NOTATION2_TYPE, " \
				"PRICE_NOTATION2, " \
				"PRICE_NOTATION3_TYPE, " \
				"PRICE_NOTATION3, " \
				"FILE_DATE"

namespace dtcc
{
	namespace database
	{
		namespace cassandra
		{
			class tradeRecordset : public recordset<int64_t, tradeRecord>
			{
			public:
				explicit tradeRecordset(const boost::shared_ptr<connector> & cnx, const std::string & table = "transactions");

				virtual std::vector<keyType> insert(const std::vector<recordType> & records);
				virtual void update(const recordType & record);
				virtual void remove(const std::string & filter = "");
				virtual void select(std::vector<recordType> & ret, const std::string & filter);
				virtual void select(const std::vector<keyType> & keys, std::vector<recordType> & ret);

			private:
				boost::shared_ptr<connector> cnx_;
				std::string table_;
			};
		}
	}
}


#endif /* DATABASE_RECORDSETS_TRADE_HPP_ */