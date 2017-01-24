#ifndef DTCC_TEST_LOGGER_HPP_
#define DTCC_TEST_LOGGER_HPP_

#include <boost/test/unit_test.hpp>

#include "application/logger.hpp"

namespace testSuite
{
	class logger
	{
	public:
		static void simpleLog();
		static boost::unit_test_framework::test_suite* suite();
	};
}
#endif
