#include "pattern.hpp"

#include "registrable/base.hpp"
#include "registrable/derived.hpp"
#include "registrable/enum.hpp"

namespace testSuite
{
	void pattern::abstractFactory()
	{
		boost::shared_ptr<base> myBase = dtcc::abstractFactory<base, std::string>::createInstance("DERIVED");

		if (myBase->foo() != "hello world!")
		{
			BOOST_ERROR("\n" << " Failed to create derived class instance");
		}
	}

	void pattern::enumManager()
	{
		std::stringstream ss;
		ss << testSuite::foo::A;
		ss << testSuite::foo::B;
		ss << testSuite::foo::C;

		if (ss.str() != "ABC")
		{
			BOOST_ERROR("\n" << " Failed to create derived class instance");
		}
	}

	boost::unit_test_framework::test_suite* pattern::suite()
	{
		boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("pattern Tests");
		suite->add(BOOST_TEST_CASE(&pattern::abstractFactory));
		return suite;
	}
}



