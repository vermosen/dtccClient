#ifndef DTCC_TEST_CONVERTER_HPP_
#define DTCC_TEST_CONVERTER_HPP_

#include <boost/test/unit_test.hpp>

namespace testSuite
{
	class converter
	{
	public:
		static void charToInt();
		static boost::unit_test_framework::test_suite* suite();
	};
}

#endif
