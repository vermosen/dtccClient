/*
* connector.hpp
*
*  Created on: May 30, 2016
*      Author: vermosen
*/

#ifndef DATABASE_CONNECTOR_HPP_
#define DATABASE_CONNECTOR_HPP_

#include <string>

#include <boost/shared_ptr.hpp>

#include <soci/soci.h>

#include "pattern/abstractFactory.hpp"

namespace dtcc
{
	namespace database
	{
		class connector
		{
		public:
			connector() {};
			virtual ~connector() {};
			
			virtual void connect() = 0;
			virtual void close() = 0;

			virtual void begin() = 0;
			virtual void commit() = 0;
			virtual void rollback() = 0;

			virtual void execute(const std::string & statement) = 0;
		};
	}
}

#endif /* CONNECTOR_HPP_ */