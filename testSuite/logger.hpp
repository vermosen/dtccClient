#ifndef DTCC_TEST_LOGGER_HPP_
#define DTCC_TEST_LOGGER_HPP_

#include <boost/test/unit_test.hpp>

#include "application/logger.hpp"

class loggerTest
{
public:
	static void testLogger()
	{
		BOOST_TEST_MESSAGE("Testing log creation...");

		try
		{
			dtcc::logger::setLogger("dtccTestSuite.log", dtcc::logger::type::Info);
			LOG_INFO() << "Test is starting";
			LOG_INFO() << "Hello World!";
		}
		catch (const std::exception&)
		{
			BOOST_ERROR("\n" << " Failed to write log!");
		}
	}
	static boost::unit_test_framework::test_suite* suite()
	{
		boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("compression Tests");
		suite->add(BOOST_TEST_CASE(&loggerTest::testLogger));
		return suite;
	}
};

#endif
