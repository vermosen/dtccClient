#ifndef DTCC_TEST_COMPRESSION_HPP_
#define DTCC_TEST_COMPRESSION_HPP_

#include <boost/test/unit_test.hpp>

namespace testSuite
{
	class compression
	{
	public:
		static void boostCompression();
		static boost::unit_test_framework::test_suite* suite();
	};
}


#endif
