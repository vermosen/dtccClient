#ifndef RECORD_HPP_
#define RECORD_HPP_

#include <string>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "pattern/abstractFactory.hpp"

namespace dtcc
{
	namespace database
	{
		class record
		{
		public:

			enum class assetType
			{
				//static registerEnum < assetType, std::string, child> register_;

				rate = 1,
				commodity = 2,
				equity = 3,
				currency = 4,
				credit = 5


			};

			enum class action
			{
				new_ = 1,
				cancel_ = 2,
				correct_ = 3
			};

			enum class collateralization
			{
				fc = 1,
				oc = 2,
				pc = 3,
				uc = 4
			};

			record() {};
			record(const std::string & line);
		private:
			int64_t DISSEMINATION_ID;
			boost::optional<int64_t> ORIGINAL_DISSEMINATION_ID;
			action ACTION;
			boost::posix_time::ptime EXECUTION_TIMESTAMP;
			bool CLEARED;
			boost::optional<int64_t> INDICATION_OF_COLLATERALIZATION;
			boost::optional<bool> INDICATION_OF_END_USER_EXCEPTION;
			bool INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
			bool BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;
			bool EXECUTION_VENUE;
			boost::gregorian::date EFFECTIVE_DATE;
			boost::gregorian::date END_DATE;
			std::string DAY_COUNT_CONVENTION;
			char SETTLEMENT_CURRENCY[3];
			assetType ASSET_CLASS;
		};
	}
}

#endif
