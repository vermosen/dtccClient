#ifndef ENUM_ACTION_HPP_
#define ENUM_ACTION_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class action : char
		{ 
			  _new		= 1
			, cancel	= 2
			, correct	= 3
		};
	}

	template <>
	const std::vector<std::pair<database::action, std::string> > 
		enumConversions<database::action>::enumToStringVector =
	{
		 { database::action::_new	, "NEW"		}
		,{ database::action::cancel	, "CANCEL"	}
		,{ database::action::correct, "CORRECT" }
	};
}

#endif
