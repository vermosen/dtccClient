#ifndef ENUM_COLLATERALIZATION_HPP_
#define ENUM_COLLATERALIZATION_HPP_

#include <iostream>

namespace dtcc
{
	namespace database
	{
		enum class collateralization
		{
			fc = 1,
			oc = 2,
			pc = 3,
			uc = 4
		};

		std::ostream & operator << (std::ostream &, const collateralization&);
	}
}

#endif
