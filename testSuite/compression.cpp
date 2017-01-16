#ifndef DTCC_TEST_COMPRESSION_CPP_
#define DTCC_TEST_COMPRESSION_CPP_

#include "compression.hpp"

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

/* remember to document new and/or updated tests in the Doxygen
comment block of the corresponding class */

void compressionTest::testBoostCompression()
{
	BOOST_TEST_MESSAGE("Testing boost compression using zlib...");

	std::string test = "hello world";
	std::stringstream sOut;
	std::stringstream sIn(test);

	{
		boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sOut);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
		in.push(boost::iostreams::zlib_compressor());
		in.push(sIn);
		boost::iostreams::copy(in, out);
	}

	sIn.clear(); sIn.seekp(0, std::ios::beg);
	sOut.seekg(0, std::ios::beg);

	{
		boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sIn);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
		in.push(boost::iostreams::zlib_decompressor());
		in.push(sOut);
		boost::iostreams::copy(in, out);
	}

	std::string res = sIn.str();

	if (test != res)
	{
		BOOST_ERROR("\n" <<
			" Failed!" <<
			" Expected result: " << test << "\n"
			" Obtained: " << res);
	}
}

boost::unit_test_framework::test_suite * compressionTest::suite()
{
	boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("compression Tests");
	suite->add(BOOST_TEST_CASE(&compressionTest::testBoostCompression));
	return suite;
}

#endif
