#include "logger.hpp"

namespace testSuite
{
	void logger::simpleLog()
	{
		BOOST_TEST_MESSAGE("Testing log creation...");

		try
		{
			dtcc::logger::initialize("dtccTestSuite.log", dtcc::severity::info);
			LOG_INFO() << "Test is starting";
			LOG_INFO() << "Hello World!";
		}
		catch (const std::exception&)
		{
			BOOST_ERROR("\n" << " Failed to write log!");
		}
	}
	boost::unit_test_framework::test_suite* logger::suite()
	{
		boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("logger Tests");
		suite->add(BOOST_TEST_CASE(&logger::simpleLog));
		return suite;
	}
}