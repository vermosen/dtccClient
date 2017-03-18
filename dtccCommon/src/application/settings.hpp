#ifndef APPLICATION_SETTINGS_HPP_
#define APPLICATION_SETTINGS_HPP_

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>

#include "database/record/enum.hpp"
#include "application/logger.hpp"

namespace dtcc
{
	struct settings
	{
		struct logger
		{
			// TODO: use the assetType factory
			std::string fileStr_;
			dtcc::severity severity_;
		};

		struct asset
		{
			// TODO: use the assetType factory
			database::assetType type_;
			std::string fileStr_;
		};

		logger logger_;
		std::string database_;
		std::string webConnector_;
		boost::gregorian::date startDate_;
		boost::gregorian::date endDate_;
		std::string baseUrl_;
		int64_t memory_;
		std::vector<asset> assets_;
	};
}


#endif