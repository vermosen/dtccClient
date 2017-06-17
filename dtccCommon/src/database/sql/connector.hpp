/*
* sqlServer.hpp
*
*  Created on: Jul 3, 2016
*      Author: vermosen
*/

#ifndef SQL_CONNECTORS_HPP_
#define SQL_CONNECTORS_HPP_

#include <vector>

#include "database/connector.hpp"

namespace dtcc
{
	namespace database
	{
		namespace sql
		{
			class connector : public database::connector
			{
			public:
				connector();
				virtual void connect();
				virtual void close();
				virtual void begin() { session_->begin(); }
				virtual void commit() { session_->commit(); }
				virtual void rollback() { session_->rollback(); }
				virtual void execute(const std::string & statement);

				void setConnectionString(const std::string &);
				boost::shared_ptr<soci::session> session() { return session_; }

				//static registerTypeNoArgs<database::connector, std::string, connector> register_;

			private:
				boost::shared_ptr<soci::session> session_;
				std::string connectionString_;
			};
		}
	}
}
#endif /* CONNECTORS_SQL_SERVER_HPP_ */