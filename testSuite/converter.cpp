#ifndef DTCC_TEST_COMPRESSION_CPP_
#define DTCC_TEST_COMPRESSION_CPP_

#include "converter.hpp"
#include "utils/converter.hpp"

void converterTest::testCharToInt()
{
	BOOST_TEST_MESSAGE("Testing conversion from char to int...");

	int32_t val = 2147381647;

	dtcc::util::converter<int32_t, char[4]> forward;
	forward.t1_ = val;

	dtcc::util::converter<char[4], int32_t> backward;
	memcpy(forward.t2_, backward.t1_, sizeof(forward.t2_));

	int32_t res = backward.t2_;

	if (val != res)
	{
		BOOST_ERROR("\n" <<
			" Failed!" <<
			" Expected result: " << val << "\n"
			" Obtained: " << res);
	}
}

boost::unit_test_framework::test_suite * converterTest::suite()
{
	boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("compression Tests");
	suite->add(BOOST_TEST_CASE(&converterTest::testCharToInt));
	return suite;
}

#endif
