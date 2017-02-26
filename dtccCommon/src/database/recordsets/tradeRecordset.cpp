#include "application/logger.hpp"
#include "database/recordsets/tradeRecordset.hpp"

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
				// step 2: insert into index and get index ids in return
				for (auto it = records.cbegin(); it != records.cend(); ++it)
				{
					int id = 0;

					connector_->session()->once
						<< "INSERT INTO dbo.dtccTransactions("
						<< " DISSEMINATION_ID, ORIGINAL_DISSEMINATION_ID, ACTION,"
						<< " EXECUTION_TIMESTAMP, CLEARED, INDICATION_OF_COLLATERALIZATION,"
						<< " INDICATION_OF_END_USER_EXCEPTION, INDICATION_OF_OTHER_PRICE_AFFECTING_TERM,"
						<< " BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS, EXECUTION_VENUE,"
						<< " EFFECTIVE_DATE, END_DATE, DAY_COUNT_CONVENTION, SETTLEMENT_CURRENCY, "
						<< " ASSET_CLASS, SUBASSET_CLASS_FOR_OTHER_COMMODITY, "
						<< " TAXONOMY, PRICE_FORMING_CONTINUATION_DATA, UNDERLYING_ASSET_1, "
						<< " PRICE_NOTATION_TYPE, PRICE_NOTATION, ADDITIONAL_PRICE_NOTATION_TYPE, "
						<< " ADDITIONAL_PRICE_NOTATION, NOTIONAL_CURRENCY_1, NOTIONAL_CURRENCY_2,"
						<< " ROUNDED_NOTIONAL_AMOUNT_1, ROUNDED_NOTIONAL_AMOUNT_1_PLUS, ROUNDED_NOTIONAL_AMOUNT_2,"
						<< " ROUNDED_NOTIONAL_AMOUNT_2_PLUS, PAYMENT_FREQUENCY_1, PAYMENT_FREQUENCY_2,"
						<< " RESET_FREQUENCY_1, RESET_FREQUENCY_2, EMBEDED_OPTION,"
						<< " OPTION_STRIKE_PRICE, OPTION_TYPE, OPTION_FAMILY,"
						<< " OPTION_CURRENCY, OPTION_PREMIUM, OPTION_LOCK_PERIOD,"
						<< " OPTION_EXPIRATION_DATE, PRICE_NOTATION2_TYPE, PRICE_NOTATION2,"
						<< " PRICE_NOTATION3_TYPE, PRICE_NOTATION3, FILE_DATE"
						<< ")"
						<< " OUTPUT INSERTED.DISSEMINATION_ID"
						<< " VALUES ("
						<< ":v1, :v2, :v3, :v4, :v5, :v6, :v7, :v8, :v9, :v10, "
						<< ":v11, :v12, :v13, :v14, :v15, :v16, :v17, :v18, :v19, :v20, "
						<< ":v21, :v22, :v23, :v24, :v25, :v26, :v27, :v28, :v29, :v30, "
						<< ":v31, :v32, :v33, :v34, :v35, :v36, :v37, :v38, :v39, :v40, "
						<< ":v41, :v42, :v43, :v44, :v45, :v46"
						<< ")",
						soci::use(it->DISSEMINATION_ID, "v1"),
						soci::use(it->ORIGINAL_DISSEMINATION_ID, "v2"),
						soci::use(static_cast<int>(it->ACTION), "v3"),
						soci::use(it->EXECUTION_TIMESTAMP, "v4"),
						soci::use(static_cast<int>(it->CLEARED), "v5"),
						soci::use(it->INDICATION_OF_COLLATERALIZATION, "v6"), 
						soci::use(it->INDICATION_OF_END_USER_EXCEPTION, "v7"),
						soci::use(it->INDICATION_OF_OTHER_PRICE_AFFECTING_TERM, "v8"),
						soci::use(it->BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS, "v9"),
						soci::use(it->EXECUTION_VENUE, "v10"),
						soci::use(it->EFFECTIVE_DATE, "v11"),
						soci::use(it->END_DATE, "v12"),
						soci::use(it->DAY_COUNT_CONVENTION, "v13"),
						soci::use(it->SETTLEMENT_CURRENCY, "v14"),
						soci::use(static_cast<int>(it->ASSET_CLASS), "v15"),
						soci::use(it->SUBASSET_CLASS_FOR_OTHER_COMMODITY, "v16"),
						soci::use(it->TAXONOMY, "v17"),
						soci::use(static_cast<int>(it->PRICE_FORMING_CONTINUATION_DATA), "v18"),
						soci::use(it->UNDERLYING_ASSET_1, "v19"),
						soci::use(it->PRICE_NOTATION_TYPE, "v20"),
						soci::use(it->PRICE_NOTATION, "v21"),
						soci::use(it->ADDITIONAL_PRICE_NOTATION_TYPE, "v22"),
						soci::use(it->ADDITIONAL_PRICE_NOTATION, "v23"),
						soci::use(it->NOTIONAL_CURRENCY_1, "v24"),
						soci::use(it->NOTIONAL_CURRENCY_2, "v25"), 
						soci::use(it->roundedNotionalAmount1(), "v26"),
						soci::use(it->roundedNotionalAmountPlus1(), "v27"),
						soci::use(it->roundedNotionalAmount2(), "v28"),
						soci::use(it->roundedNotionalAmountPlus2(), "v29"),
						soci::use(it->PAYMENT_FREQUENCY_1, "v30"),
						soci::use(it->PAYMENT_FREQUENCY_2, "v31"),
						soci::use(it->RESET_FREQUENCY_1, "v32"),
						soci::use(it->RESET_FREQUENCY_2, "v33"),
						soci::use(it->EMBEDED_OPTION, "v34"),
						soci::use(it->OPTION_STRIKE_PRICE, "v35"),
						soci::use(it->OPTION_TYPE, "v36"),
						soci::use(it->OPTION_FAMILY, "v37"),
						soci::use(it->OPTION_CURRENCY, "v38"),
						soci::use(it->OPTION_PREMIUM, "v39"),
						soci::use(it->OPTION_LOCK_PERIOD, "v40"),
						soci::use(it->OPTION_EXPIRATION_DATE, "v41"),
						soci::use(it->PRICE_NOTATION2_TYPE, "v42"),
						soci::use(it->PRICE_NOTATION2, "v43"),
						soci::use(it->PRICE_NOTATION3_TYPE, "v44"),
						soci::use(it->PRICE_NOTATION3, "v45"),
						soci::use(it->FILE_DATE, "v46"),
						soci::into(id);

					ids.push_back(id);
				}

				connector_->session()->commit();
			}
			catch (const std::exception & ex)
			{
				LOG_ERROR() << "Failed to insert data in the database: " << ex.what();
				connector_->session()->rollback();
				return std::vector<keyType>();
			}

			return ids;
		}
		void tradeRecordset::update(const recordType & record)
		{

		}
		void tradeRecordset::remove(const recordType & record)
		{

		}
		void tradeRecordset::select(std::vector<recordType> & ret, const std::string & filter)
		{

		}
		void tradeRecordset::select(const std::vector<keyType> & keys, std::vector<recordType> & ret)
		{

		}
	}
}
