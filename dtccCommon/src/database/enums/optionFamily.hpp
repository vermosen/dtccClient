#ifndef ENUM_OPTION_FAMILY_HPP_
#define ENUM_OPTION_FAMILY_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class optionFamily
		{
			  american
			, european
			, asian
		};
	}

	template <>
	const std::vector<std::pair<database::optionFamily, std::string> >
		enumConversions<database::optionFamily>::enumToStringVector =
	{
		 { database::optionFamily::american	, "AM" }
		,{ database::optionFamily::european	, "EU" }
		,{ database::optionFamily::asian	, "AS" }
	};
}

#endif