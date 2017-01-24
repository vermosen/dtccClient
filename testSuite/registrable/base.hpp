#ifndef REGISTRABLE_BASE_HPP_
#define REGISTRABLE_BASE_HPP_

#include "pattern/abstractFactory.hpp"

namespace testSuite
{
	class base
	{
	public:
		virtual std::string foo() = 0;
	};
}

#endif
