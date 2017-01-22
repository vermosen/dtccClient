#ifndef DTCC_TEST_CONVERTER_HPP_
#define DTCC_TEST_CONVERTER_HPP_

#include <boost/test/unit_test.hpp>

class converterTest
{
public:
	static void testCharToInt();
	static boost::unit_test_framework::test_suite* suite();
};

#endif
