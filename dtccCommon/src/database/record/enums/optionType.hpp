#ifndef ENUM_OPTION_TYPE_HPP_
#define ENUM_OPTION_TYPE_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class optionType : char
		{
			  cm = 1
			, cf = 2
			, dm = 3
			, fm = 4
			, nc = 5
			, pm = 6
			, pc = 7
			, pf = 8
			, rf = 9
			, xx = 10
		};
	}

	template <>
	const std::vector<std::pair<database::optionType, std::string> >
		enumConversions<database::optionType>::enumToStringVector =
	{
		 { database::optionType::cm	, "C-" }
		,{ database::optionType::cf	, "CF" }
		,{ database::optionType::dm	, "D-" }
		,{ database::optionType::fm	, "F-" }
		,{ database::optionType::nc	, "NC" }
		,{ database::optionType::pm	, "P-" }
		,{ database::optionType::pc	, "PC" }
		,{ database::optionType::pf	, "PF" }
		,{ database::optionType::rf	, "RF" }
		,{ database::optionType::xx	, "XX" }
	};
}

#endif