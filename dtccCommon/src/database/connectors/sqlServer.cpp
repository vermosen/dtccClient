#include "sqlServer.hpp"

#include <soci/odbc/soci-odbc.h>

namespace dtcc
{
	namespace database
	{
		sqlServer::sqlServer() : connector() {}

		void sqlServer::connect(const std::string & connectionString)
		{
			session_ = boost::shared_ptr<soci::session>(
				new soci::session(soci::odbc, connectionString));
		}
	}
}