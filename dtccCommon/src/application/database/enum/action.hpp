#ifndef ENUM_ACTION_HPP_
#define ENUM_ACTION_HPP_

#include <iostream>

namespace dtcc
{
	namespace database
	{
		enum class action
		{
			new_	= 1,
			cancel	= 2,
			correct	= 3
		};

		std::ostream & operator << (std::ostream &, const action&);
	}
}

#endif
