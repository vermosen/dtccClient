#include "application/logger.hpp"
#include "database/recordsets/tradeRecordset.hpp"

namespace dtcc
{
	namespace database
	{

		using ::operator<<;

		tradeRecordset::tradeRecordset(boost::shared_ptr<connector> conn) : recordset<keyType, recordType>(conn) {}

		std::vector<tradeRecordset::keyType> tradeRecordset::insert(const std::vector<tradeRecordset::recordType> & records)
		{
			std::vector<keyType> ids;

			connector_->session()->begin();

			try
			{
				// step 2: insert into index and get index ids in return
				for (auto it = records.cbegin(); it != records.cend(); ++it)
				{
					int id;

					connector_->session()->once
						<< "INSERT INTO dbo.testTable (text)"
						<< " VALUES 'toto'"
						<< " RETURNING id",
						soci::use(it->TAXONOMY, "toto"),
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
