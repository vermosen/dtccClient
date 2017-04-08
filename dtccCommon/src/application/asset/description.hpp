#pragma once
#ifndef APPLICATION_ASSET_DESCRIPTION_HPP_
#define APPLICATION_ASSET_DESCRIPTION_HPP_

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>

#include "database/record/enum.hpp"
#include "application/logger.hpp"

namespace dtcc
{
	namespace asset
	{
		struct description
		{
			database::assetType type_;
			std::string fileStr_;
		};
	}
}


#endif
