#ifndef RECORD_HPP_
#define RECORD_HPP_

#include <string>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "pattern/abstractFactory.hpp"

#include "application/database/enum/action.hpp"
#include "application/database/enum/assetType.hpp"
#include "application/database/enum/collateralization.hpp"

namespace dtcc
{
	namespace database
	{
		class record
		{
		public:

			record() {};
			record(const std::string & line);
		private:
			int64_t DISSEMINATION_ID;
			boost::optional<int64_t>	ORIGINAL_DISSEMINATION_ID;
			action						ACTION;
			boost::posix_time::ptime	EXECUTION_TIMESTAMP;
			bool						CLEARED;
			boost::optional<int64_t>	INDICATION_OF_COLLATERALIZATION;
			boost::optional<bool>		INDICATION_OF_END_USER_EXCEPTION;
			bool						INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
			bool						BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;
			bool						EXECUTION_VENUE;
			boost::gregorian::date		EFFECTIVE_DATE;
			boost::gregorian::date		END_DATE;
			std::string					DAY_COUNT_CONVENTION;
			std::array<char, 3>			SETTLEMENT_CURRENCY;
			assetType					ASSET_CLASS;
		};
	}
}

#endif
