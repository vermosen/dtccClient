#ifndef ENUM_CLEARED_HPP_
#define ENUM_CLEARED_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class cleared : bool
		{
			  cleared   = 1
			, uncleared = 0
		};
	}

	template <>
	const std::vector<std::pair<database::cleared, std::string> >
		enumConversions<database::cleared>::enumToStringVector =
	{
		 { database::cleared::cleared	, "C" }
		,{ database::cleared::uncleared	, "U" }
	};
}

#endif
