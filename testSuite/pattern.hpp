#ifndef DTCC_TEST_PATTERN_HPP_
#define DTCC_TEST_PATTERN_HPP_

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "pattern/abstractFactory.hpp"
#include "pattern/enumManager.hpp"

namespace testSuite
{
	class pattern
	{
	public:
		static void abstractFactory();
		static void enumManager();
		static boost::unit_test_framework::test_suite* suite();
	};
}
#endif
