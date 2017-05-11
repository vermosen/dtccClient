/*
* recordset.hpp
*
*  Created on: Jul 3, 2016
*      Author: vermosen
*/

#ifndef CONNECTORS_SQL_SERVER_HPP_
#define CONNECTORS_SQL_SERVER_HPP_

#include <vector>

#include "database/connector.hpp"

namespace dtcc
{
	namespace database
	{
		class sqlServer : public connector
		{
		public:
			sqlServer();
			void connect(const std::string & connectionString);
			void close();

			static registerTypeNoArgs<connector, std::string, sqlServer> register_;
		};
	}
}
#endif /* CONNECTORS_SQL_SERVER_HPP_ */