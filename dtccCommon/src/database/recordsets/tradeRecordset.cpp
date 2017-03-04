#include "application/logger.hpp"
#include "database/recordsets/tradeRecordset.hpp"
#include "database/tradeConverter.hpp"
namespace dtcc
{
	namespace database
	{

		using ::operator<<;

		tradeRecordset::tradeRecordset(boost::shared_ptr<connector> conn) 
			: recordset<keyType, recordType>(conn) {}

		std::vector<tradeRecordset::keyType> tradeRecordset::insert(
			const std::vector<tradeRecordset::recordType> & records)
		{
			std::vector<keyType> ids;
			connector_->session()->begin();

			try
			{
				LOG_INFO() << "Trying to insert " << records.size() << " in the database"; 

				// step 2: insert into index and get index ids in return
				for (auto it = records.cbegin(); it != records.cend(); ++it)
				{
					int id = 0;

					connector_->session()->once
						<< "INSERT INTO dbo.dtccTransactions("
						<< " DISSEMINATION_ID,"
						<< " ORIGINAL_DISSEMINATION_ID,"
						<< " ACTION,"
						<< " EXECUTION_TIMESTAMP,"
						<< " CLEARED,"
						<< " INDICATION_OF_COLLATERALIZATION,"
						<< " INDICATION_OF_END_USER_EXCEPTION,"
						<< " INDICATION_OF_OTHER_PRICE_AFFECTING_TERM,"
						<< " BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS,"
						<< " EXECUTION_VENUE,"
						<< " EFFECTIVE_DATE,"
						<< " END_DATE,"
						<< " DAY_COUNT_CONVENTION,"
						<< " SETTLEMENT_CURRENCY, "
						<< " ASSET_CLASS,"
						<< " SUBASSET_CLASS_FOR_OTHER_COMMODITY, "
						<< " TAXONOMY,"
						<< " PRICE_FORMING_CONTINUATION_DATA,"
						<< " UNDERLYING_ASSET_1, "
						<< " UNDERLYING_ASSET_2, "
						<< " PRICE_NOTATION_TYPE,"
						<< " PRICE_NOTATION,"
						<< " ADDITIONAL_PRICE_NOTATION_TYPE, "
						<< " ADDITIONAL_PRICE_NOTATION,"
						<< " NOTIONAL_CURRENCY_1,"
						<< " NOTIONAL_CURRENCY_2,"
						<< " ROUNDED_NOTIONAL_AMOUNT_1,"
						<< " ROUNDED_NOTIONAL_AMOUNT_1_PLUS,"
						<< " ROUNDED_NOTIONAL_AMOUNT_2,"
						<< " ROUNDED_NOTIONAL_AMOUNT_2_PLUS,"
						<< " PAYMENT_FREQUENCY_1,"
						<< " PAYMENT_FREQUENCY_2,"
						<< " RESET_FREQUENCY_1,"
						<< " RESET_FREQUENCY_2,"
						<< " EMBEDED_OPTION,"
						<< " OPTION_STRIKE_PRICE,"
						<< " OPTION_TYPE,"
						<< " OPTION_FAMILY,"
						<< " OPTION_CURRENCY,"
						<< " OPTION_PREMIUM,"
						<< " OPTION_LOCK_PERIOD,"
						<< " OPTION_EXPIRATION_DATE,"
						<< " PRICE_NOTATION2_TYPE,"
						<< " PRICE_NOTATION2,"
						<< " PRICE_NOTATION3_TYPE,"
						<< " PRICE_NOTATION3,"
						<< " FILE_DATE"
						<< ")"
						<< " OUTPUT INSERTED.DISSEMINATION_ID"
						<< " VALUES ("
						<< " :DISSEMINATION_ID,"
						<< " :ORIGINAL_DISSEMINATION_ID,"
						<< " :ACTION,"
						<< " :EXECUTION_TIMESTAMP,"
						<< " :CLEARED,"
						<< " :INDICATION_OF_COLLATERALIZATION,"
						<< " :INDICATION_OF_END_USER_EXCEPTION,"
						<< " :INDICATION_OF_OTHER_PRICE_AFFECTING_TERM,"
						<< " :BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS,"
						<< " :EXECUTION_VENUE,"
						<< " :EFFECTIVE_DATE,"
						<< " :END_DATE,"
						<< " :DAY_COUNT_CONVENTION,"
						<< " :SETTLEMENT_CURRENCY, "
						<< " :ASSET_CLASS,"
						<< " :SUBASSET_CLASS_FOR_OTHER_COMMODITY, "
						<< " :TAXONOMY,"
						<< " :PRICE_FORMING_CONTINUATION_DATA,"
						<< " :UNDERLYING_ASSET_1, "
						<< " :UNDERLYING_ASSET_2, "
						<< " :PRICE_NOTATION_TYPE,"
						<< " :PRICE_NOTATION,"
						<< " :ADDITIONAL_PRICE_NOTATION_TYPE, "
						<< " :ADDITIONAL_PRICE_NOTATION,"
						<< " :NOTIONAL_CURRENCY_1,"
						<< " :NOTIONAL_CURRENCY_2,"
						<< " :ROUNDED_NOTIONAL_AMOUNT_1,"
						<< " :ROUNDED_NOTIONAL_AMOUNT_1_PLUS,"
						<< " :ROUNDED_NOTIONAL_AMOUNT_2,"
						<< " :ROUNDED_NOTIONAL_AMOUNT_2_PLUS,"
						<< " :PAYMENT_FREQUENCY_1,"
						<< " :PAYMENT_FREQUENCY_2,"
						<< " :RESET_FREQUENCY_1,"
						<< " :RESET_FREQUENCY_2,"
						<< " :EMBEDED_OPTION,"
						<< " :OPTION_STRIKE_PRICE,"
						<< " :OPTION_TYPE,"
						<< " :OPTION_FAMILY,"
						<< " :OPTION_CURRENCY,"
						<< " :OPTION_PREMIUM,"
						<< " :OPTION_LOCK_PERIOD,"
						<< " :OPTION_EXPIRATION_DATE,"
						<< " :PRICE_NOTATION2_TYPE,"
						<< " :PRICE_NOTATION2,"
						<< " :PRICE_NOTATION3_TYPE,"
						<< " :PRICE_NOTATION3,"
						<< " :FILE_DATE"
						<< ")",
						soci::use(*it),
						soci::into(id);

					ids.push_back(id);
				}

				connector_->session()->commit();
			}
			catch (const std::exception & ex)
			{
				LOG_ERROR() << "Failed to insert data in the database: " << ex.what();
				connector_->session()->rollback();
				
				// TODO: second chance insert
				return std::vector<keyType>();
			}

			return ids;
		}

		void tradeRecordset::update(const recordType & record)
		{

		}
		void tradeRecordset::remove(const std::string & filter)
		{
			connector_->session()->begin();

			try
			{
				// remove the records according to the filter passed
				soci::statement st = (connector_->session()->prepare 
					<< "delete from dbo.dtccTransactions where (" << filter << ")");

				st.execute(true);

				if (!st.get_affected_rows())
				{
					LOG_INFO() << "None of the rows were affected";
				}
				else
				{
					connector_->session()->commit();
					LOG_INFO() << st.get_affected_rows() << " rows have been successfully deleted";
				}
			}
			catch (const std::exception & ex)
			{
				LOG_ERROR() << "Failed to remove the selected records from the database: " << ex.what();
				connector_->session()->rollback();
			}
		}
		void tradeRecordset::select(std::vector<recordType> & ret, const std::string & filter)
		{

		}
		void tradeRecordset::select(const std::vector<keyType> & keys, std::vector<recordType> & ret)
		{

		}
	}
}
