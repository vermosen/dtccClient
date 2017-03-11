#ifndef APPLICATION_SETTINGS_HPP_
#define APPLICATION_SETTINGS_HPP_

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>

#include "record/enum.hpp"

namespace dtcc
{
	struct settings2
	{
		boost::gregorian::date startDate_;
		boost::gregorian::date endDate_;
		std::string baseUrl_;
		int64_t memory_;
		std::vector<std::string> assets_;
	};

	struct settings
	{
		struct asset
		{
			// TODO: use the assetType factory
			database::assetType type_;
			std::string fileStr_;
		};

		boost::gregorian::date start_;
		boost::gregorian::date end_;
		std::vector<asset> assets_;
		std::string baseUrl_;
		int64_t memory_;
	};
}


#endif