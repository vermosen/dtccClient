#ifndef ENUM_ASSET_TYPE_HPP_
#define ENUM_ASSET_TYPE_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class assetType 
		{ 
			  interestRate
			, commodity
			, equity
			, currency
			, credit 
		};
	}

	template <>
	const std::vector<std::pair<database::assetType, std::string> > 
		enumConversions<database::assetType>::enumToStringVector =
	{
		  { database::assetType::interestRate	, "IR" }
		, { database::assetType::commodity		, "CO" }
		, { database::assetType::equity			, "EQ" }
		, { database::assetType::currency		, "CU" }
		, { database::assetType::credit			, "CR" }
	};
}

#endif
