#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <boost/test/unit_test.hpp>
#include <boost/test/framework.hpp>

#include "compression.hpp"
#include "converter.hpp"
#include "logger.hpp"

boost::unit_test::test_suite* init_unit_test_suite(int, char*[])
{

	int argc = boost::unit_test::framework::master_test_suite().argc;
	char **argv = boost::unit_test::framework::master_test_suite().argv;

	std::ostringstream header;
	header << "launching dtcc test suite " << std::endl;

	std::string rule = std::string(41, '=');
	BOOST_TEST_MESSAGE(rule);
	BOOST_TEST_MESSAGE(header.str());
	BOOST_TEST_MESSAGE(rule);

	boost::unit_test::test_suite* test = BOOST_TEST_SUITE("Dtcc test suite");

	// individual tests
	test->add(loggerTest::suite());
	test->add(compressionTest::suite());
	test->add(converterTest::suite());

	return test;
}

#endif