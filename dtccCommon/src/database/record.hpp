#ifndef RECORD_HPP_
#define RECORD_HPP_

#include <string>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "pattern/abstractFactory.hpp"

#include "database/enum.hpp"

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
			int64_t									DISSEMINATION_ID;
			boost::optional<int64_t>				ORIGINAL_DISSEMINATION_ID;
			action									ACTION;
			boost::posix_time::ptime				EXECUTION_TIMESTAMP;
			bool									CLEARED;
			boost::optional<collateralization>		INDICATION_OF_COLLATERALIZATION;
			boost::optional<bool>					INDICATION_OF_END_USER_EXCEPTION;
			bool									INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
			bool									BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;
			boost::optional<bool>					EXECUTION_VENUE;
			boost::optional<boost::gregorian::date>	EFFECTIVE_DATE;
			boost::optional<boost::gregorian::date> END_DATE;
			std::string								DAY_COUNT_CONVENTION;
			boost::optional<std::array<char, 3> >	SETTLEMENT_CURRENCY;
			assetType								ASSET_CLASS;
			std::string								SUBASSET_CLASS_FOR_OTHER_COMMODITY;
			std::string								TAXONOMY;
			priceFormingContinuationData			PRICE_FORMING_CONTINUATION_DATA;
			std::string								UNDERLYING_ASSET_1;
			std::string								UNDERLYING_ASSET_2;
			std::string								PRICE_NOTATION_TYPE;
			boost::optional<double>					PRICE_NOTATION;
			std::string								ADDITIONAL_PRICE_NOTATION_TYPE;
			std::string								ADDITIONAL_PRICE_NOTATION;
			std::string								NOTIONAL_CURRENCY_1;
			std::string								NOTIONAL_CURRENCY_2;
			boost::optional<double>					ROUNDED_NOTIONAL_AMOUNT_1;
			bool									ROUNDED_NOTIONAL_AMOUNT_1_PLUS;
			boost::optional<double>					ROUNDED_NOTIONAL_AMOUNT_2;
			bool									ROUNDED_NOTIONAL_AMOUNT_2_PLUS;
			std::string								PAYMENT_FREQUENCY_1;
			std::string								PAYMENT_FREQUENCY_2;
			std::string								RESET_FREQUENCY_1;
			std::string								RESET_FREQUENCY_2;
			bool									EMBEDED_OPTION;
			boost::optional<double>					OPTION_STRIKE_PRICE;
			std::string								OPTION_TYPE; // TODO: find a taxonomy
			boost::optional<optionFamily>			OPTION_FAMILY;
			boost::optional<std::array<char, 3> >	OPTION_CURRENCY;
			boost::optional<double>					OPTION_PREMIUM;
			boost::optional<boost::gregorian::date> OPTION_LOCK_PERIOD;
			boost::optional<boost::gregorian::date> OPTION_EXPIRATION_DATE;
			std::string								PRICE_NOTATION_TYPE_2;
			boost::optional<double>					PRICE_NOTATION_2;
			std::string								PRICE_NOTATION_TYPE_3;
			boost::optional<double>					PRICE_NOTATION_3;
		};
	}
}

#endif
