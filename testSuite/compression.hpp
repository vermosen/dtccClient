#ifndef DTCC_TEST_COMPRESSION_HPP_
#define DTCC_TEST_COMPRESSION_HPP_

#include <boost/test/unit_test.hpp>

class compressionTest
{
public:
	static void testBoostCompression();
	static boost::unit_test_framework::test_suite* suite();
};

#endif
