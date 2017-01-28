#ifndef TEST_ENUM_HPP_
#define TEST_ENUM_HPP_

#include "pattern/enumManager.hpp"

namespace testSuite
{
	enum class foo
	{
		  A
		, B
		, C
	};
}

template <>
const std::vector<std::pair<testSuite::foo, std::string> >
dtcc::enumConversions<testSuite::foo>::enumToStringVector =
{
	 { testSuite::foo::A, "A" }
	,{ testSuite::foo::B, "B" }
	,{ testSuite::foo::C, "C" }
};

#endif