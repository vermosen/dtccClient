#ifndef ENUM_ASSET_TYPE_HPP_
#define ENUM_ASSET_TYPE_HPP_

#include <iostream>

namespace dtcc
{
	namespace database
	{
		enum class assetType
		{
			rate = 1,
			commodity = 2,
			equity = 3,
			currency = 4,
			credit = 5
		};

		std::ostream & operator << (std::ostream &, const assetType&);
	}
}

#endif
