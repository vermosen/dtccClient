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

namespace dtcc
{
	namespace database
	{
		class connector
		{
		public:
			connector() {};
			virtual ~connector() {};
			virtual void connect(const std::string & connectionString) = 0;
			
			boost::shared_ptr<soci::session> session() { return session_; }

			void begin() { session_->begin(); }
			void commit() { session_->commit(); }
			void rollback() { session_->rollback(); }

		protected:
			boost::shared_ptr<soci::session> session_;
		};
	}
}

#endif /* CONNECTOR_HPP_ */