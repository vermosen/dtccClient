#ifndef ENUM_ACTION_HPP_
#define ENUM_ACTION_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class action 
		{ 
			  _new
			, cancel
			, correct 
		};
	}

	template <>
	const std::vector<std::pair<database::action, std::string> > 
		enumConversions<database::action>::enumToStringVector =
	{
		 { database::action::_new	, "NEW"		}
		,{ database::action::cancel	, "DELETE"	}
		,{ database::action::correct, "CORRECT" }
	};
}

#endif
