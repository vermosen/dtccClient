#ifndef ENUM_ASSET_TYPE_HPP_
#define ENUM_ASSET_TYPE_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class assetType : char
		{ 
			  commodity		= 1
			, credit		= 2
			, equity		= 3
			, currency		= 4
			, interestRate	= 5
		};
	}

	template <>
	const std::vector<std::pair<database::assetType, std::string> > 
		enumConversions<database::assetType>::enumToStringVector =
	{
		  { database::assetType::interestRate	, "IR" }
		, { database::assetType::commodity		, "CO" }
		, { database::assetType::equity			, "EQ" }
		, { database::assetType::currency		, "FX" }
		, { database::assetType::credit			, "CR" }
	};
}

#endif
