#ifndef APPLICATION_SETTINGS_HPP_
#define APPLICATION_SETTINGS_HPP_

#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>

#include "database/record/enum.hpp"
#include "application/logger.hpp"
#include "application/asset/description.hpp"

namespace dtcc
{
	struct settings
	{
		struct logger
		{
			std::string fileStr_;
			dtcc::severity severity_;
		};

		struct worker
		{
			struct connector
			{
				std::string type_;
				std::string protocol_;
				std::string host_;
				int port_;
				size_t bufferSize_;
			};

			connector connector_;
			dtcc::asset::description description_;
			int timeoutAfterSuccess_;
			int timeoutAfterFailure_;
			int maxAttempt_;
		};

		struct connectionString
		{
			std::string database_;
		};

		logger logger_;
		connectionString database_;
		size_t memory_;
		std::vector<worker> workers_;
	};
}


#endif