#ifndef ENUM_COLLATERALIZATION_HPP_
#define ENUM_COLLATERALIZATION_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class collateralization : char
		{ 
			  fc = 1
			, oc = 2
			, pc = 3
			, uc = 4
		};
	}

	template <>
	const std::vector<std::pair<database::collateralization, std::string> > 
		enumConversions<database::collateralization>::enumToStringVector =
	{
		 { database::collateralization::fc, "FC" }
		,{ database::collateralization::oc, "OC" }
		,{ database::collateralization::pc, "PC" }
		,{ database::collateralization::uc, "UC" }
	};
}

#endif
