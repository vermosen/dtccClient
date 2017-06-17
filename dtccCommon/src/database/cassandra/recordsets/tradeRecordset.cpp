#include "application/logger.hpp"
#include "database/cassandra/recordsets/tradeRecordset.hpp"

namespace dtcc
{
	namespace database
	{
		namespace cassandra
		{
			using ::operator<<;

			tradeRecordset::tradeRecordset(const boost::shared_ptr<connector> & cnx, const std::string & table)
				: recordset<keyType, recordType>() 
				, cnx_(cnx)
				, table_(table) {}

			std::vector<tradeRecordset::keyType> tradeRecordset::insert(
				const std::vector<tradeRecordset::recordType> & records)
			{
				std::vector<keyType> ids;

				// prepare the statement 
				;

				std::ostringstream os;

				os << "INSERT INTO "
					<< cnx_->keyspace()
					<< "."
					<< table_
					<<	" (" FIELDS ") VALUES ("
						"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
						"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
						"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
						"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
						"?, ?, ?, ?, ?, ?, ?)";

				const char* query = "INSERT INTO examples.pairs (key, value) VALUES (?, ?)";

				future ft;
				ft.inst_ = cass_session_prepare(cnx_->session(), os.str().c_str());

				cass_future_wait(ft.inst_);

				if (cass_future_error_code(ft.inst_) != CASS_OK)
				{
					LOG_ERROR() << ft.error();
				}
				else 
				{
					//* prepared = cass_future_get_prepared(ft.inst_);
				}

				return ids;
			}

			void tradeRecordset::update(const recordType & record)
			{

			}

			void tradeRecordset::remove(const std::string & filter)
			{
				//connector_->session()->begin();

				//try
				//{
				//	// remove the records according to the filter passed
				//	soci::statement st = (connector_->session()->prepare
				//		<< "delete from dbo.dtccTransactions where (" << filter << ")");

				//	st.execute(true);

				//	if (!st.get_affected_rows())
				//	{
				//		LOG_INFO() << "None of the rows were affected";
				//	}
				//	else
				//	{
				//		connector_->session()->commit();
				//		LOG_INFO() << st.get_affected_rows() << " rows have been successfully deleted";
				//	}
				//}
				//catch (const std::exception & ex)
				//{
				//	LOG_ERROR() << "Failed to remove the selected records from the database: " << ex.what();
				//	connector_->session()->rollback();
				//}
			}
			void tradeRecordset::select(std::vector<recordType> & ret, const std::string & filter)
			{

			}
			void tradeRecordset::select(const std::vector<keyType> & keys, std::vector<recordType> & ret)
			{

			}
		}
	}
}
