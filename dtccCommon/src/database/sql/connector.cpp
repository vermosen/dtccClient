#include "connector.hpp"

#include <soci/odbc/soci-odbc.h>

namespace dtcc
{
	namespace database
	{
		namespace sql
		{
			//registerTypeNoArgs<database::connector, std::string, connector>
			//	connector::register_(std::string("sql server"));

			connector::connector() : database::connector() {}

			void connector::connect()
			{
				session_ = boost::shared_ptr<soci::session>(
					new soci::session(soci::odbc, connectionString_));
			}

			void connector::close()
			{
				session_->close();
			}

			void connector::setConnectionString(const std::string & connectionString)
			{
				connectionString_ = connectionString;
			}

			void connector::execute(const std::string & statement)
			{
				soci::statement(session_->prepare << statement).execute(true);
			}
		}
	}
}