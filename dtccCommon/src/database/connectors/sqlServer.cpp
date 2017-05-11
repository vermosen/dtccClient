#include "sqlServer.hpp"

#include <soci/odbc/soci-odbc.h>

namespace dtcc
{
	namespace database
	{
		registerTypeNoArgs<connector, std::string, sqlServer>
			sqlServer::register_(std::string("sql server"));

		sqlServer::sqlServer() : connector() {}

		void sqlServer::connect(const std::string & connectionString)
		{
			session_ = boost::shared_ptr<soci::session>(
				new soci::session(soci::odbc, connectionString));
		}

		void sqlServer::close()
		{
			session_->close();
		}
	}
}