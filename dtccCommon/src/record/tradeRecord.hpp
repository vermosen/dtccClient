#ifndef TRADE_RECORD_HPP_
#define TRADE_RECORD_HPP_

#include <string>

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "pattern/abstractFactory.hpp"

#include "enum.hpp"

namespace dtcc
{
	namespace database
	{
		typedef boost::posix_time::ptime tTime;
		typedef boost::optional<boost::gregorian::date> tOptDate;
		typedef boost::optional<tTime> tOptTime;
		typedef boost::optional<std::string> tOptCcy;
		typedef boost::optional<boost::tuple<double, bool> > tOptNomPlus;
		typedef boost::optional<optionFamily> tOptFamily;
		typedef boost::optional<collateralization> tOptCollat;
		typedef boost::optional<optionType> tOptType;

		class tradeRecord
		{
			friend class tradeRecordset;
		public:
			int								DISSEMINATION_ID;
			boost::optional<int>			ORIGINAL_DISSEMINATION_ID;
			action							ACTION;
			tTime							EXECUTION_TIMESTAMP;
			cleared							CLEARED;
			tOptCollat						INDICATION_OF_COLLATERALIZATION;
			boost::optional<bool>			INDICATION_OF_END_USER_EXCEPTION;
			bool							INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
			bool							BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;	// 1 null found in https://kgc0418-tdw-data-0.s3.amazonaws.com/slices/CUMULATIVE_FOREX_2017_02_20.zip
			boost::optional<bool>			EXECUTION_VENUE;
			tOptDate						EFFECTIVE_DATE;
			tOptDate						END_DATE;
			std::string						DAY_COUNT_CONVENTION;
			tOptCcy							SETTLEMENT_CURRENCY;
			assetType						ASSET_CLASS;
			std::string						SUBASSET_CLASS_FOR_OTHER_COMMODITY;
			std::string						TAXONOMY;
			priceFormingContinuationData	PRICE_FORMING_CONTINUATION_DATA;
			std::string						UNDERLYING_ASSET_1;
			std::string						UNDERLYING_ASSET_2;
			std::string						PRICE_NOTATION_TYPE;
			boost::optional<double>			PRICE_NOTATION;
			std::string						ADDITIONAL_PRICE_NOTATION_TYPE;
			boost::optional<double>			ADDITIONAL_PRICE_NOTATION;
			std::string						NOTIONAL_CURRENCY_1;
			std::string						NOTIONAL_CURRENCY_2;
			tOptNomPlus						ROUNDED_NOTIONAL_AMOUNT_1;
			tOptNomPlus						ROUNDED_NOTIONAL_AMOUNT_2;
			std::string						PAYMENT_FREQUENCY_1;
			std::string						PAYMENT_FREQUENCY_2;
			std::string						RESET_FREQUENCY_1;
			std::string						RESET_FREQUENCY_2;
			bool							EMBEDED_OPTION;
			boost::optional<double>			OPTION_STRIKE_PRICE;
			tOptType						OPTION_TYPE;
			tOptFamily						OPTION_FAMILY;
			tOptCcy							OPTION_CURRENCY;
			boost::optional<double>			OPTION_PREMIUM;
			tOptDate						OPTION_LOCK_PERIOD;
			tOptDate						OPTION_EXPIRATION_DATE;
			std::string						PRICE_NOTATION2_TYPE;
			boost::optional<double>			PRICE_NOTATION2;
			std::string						PRICE_NOTATION3_TYPE;
			boost::optional<double>			PRICE_NOTATION3;
			boost::gregorian::date			FILE_DATE;

			boost::optional<double> roundedNotionalAmount1() const 
			{
				if (ROUNDED_NOTIONAL_AMOUNT_1 == boost::none)
					return boost::none;
				else
					return ROUNDED_NOTIONAL_AMOUNT_1->get<0>();
			}

			boost::optional<bool> roundedNotionalAmountPlus1() const
			{
				if (ROUNDED_NOTIONAL_AMOUNT_1 == boost::none)
					return boost::none;
				else
					return ROUNDED_NOTIONAL_AMOUNT_1->get<1>();
			}

			boost::optional<double> roundedNotionalAmount2() const
			{
				if (ROUNDED_NOTIONAL_AMOUNT_2 == boost::none)
					return boost::none;
				else
					return ROUNDED_NOTIONAL_AMOUNT_2->get<0>();
			}

			boost::optional<bool> roundedNotionalAmountPlus2() const
			{
				if (ROUNDED_NOTIONAL_AMOUNT_2 == boost::none)
					return boost::none;
				else
					return ROUNDED_NOTIONAL_AMOUNT_2->get<1>();
			}
		};
	}
}

#endif
