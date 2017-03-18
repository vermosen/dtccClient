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
			std::string fileStr_;
			dtcc::severity severity_;
		};

		struct asset
		{
			database::assetType type_;
			std::string fileStr_;
		};

		struct connector
		{
			std::string type_;
			std::string protocol_;
			std::string host_;
			int port_;
			size_t bufferSize_;
		};

		logger logger_;
		connector connector_;
		std::string database_;
		boost::gregorian::date startDate_;
		boost::gregorian::date endDate_;
		int64_t memory_;
		std::vector<asset> assets_;
	};
}


#endif