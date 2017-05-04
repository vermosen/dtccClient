#ifndef DTCC_TEST_PARSING_HPP_
#define DTCC_TEST_PARSING_HPP_

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

namespace testSuite
{
	class parsing
	{
	public:
		static void multipleRecords();
		static void preference();

		static boost::unit_test_framework::test_suite* suite();
	};
}
#endif
