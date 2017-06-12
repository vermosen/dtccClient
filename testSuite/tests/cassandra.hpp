#ifndef DTCC_TEST_CASSANDRA_HPP_
#define DTCC_TEST_CASSANDRA_HPP_

#include <boost/test/unit_test.hpp>

#include "database/cassandra/connector.hpp"

namespace testSuite
{
	class cassandra
	{
	public:
		static void connect();
		static void createTable();
		static void deleteTable();
		static void disconnect();
		static boost::unit_test_framework::test_suite* suite();

	private:
		static boost::shared_ptr<dtcc::database::cassandra::connector> cnx_;
	};
}


#endif
