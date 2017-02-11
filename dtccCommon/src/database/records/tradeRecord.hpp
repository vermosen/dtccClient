#ifndef TRADE_RECORD_HPP_
#define TRADE_RECORD_HPP_

#include <string>

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "pattern/abstractFactory.hpp"

#include "database/enum.hpp"

namespace dtcc
{
	namespace database
	{
		typedef boost::optional<boost::gregorian::date> tOptDate;
		typedef boost::posix_time::ptime tTime;
		//typedef boost::optional<std::array<char, 3> > tOptCcy;
		typedef boost::optional<std::string> tOptCcy;
		typedef boost::optional<boost::tuple<double, bool> > tOptNomPlus;

		class tradeRecord
		{
			friend class tradeRecordset;
		public:
			int									DISSEMINATION_ID;
			boost::optional<int>				ORIGINAL_DISSEMINATION_ID;
			action								ACTION;
			tTime								EXECUTION_TIMESTAMP;
			cleared								CLEARED;
			collateralization					INDICATION_OF_COLLATERALIZATION;
			boost::optional<bool>				INDICATION_OF_END_USER_EXCEPTION;
			bool								INDICATION_OF_OTHER_PRICE_AFFECTING_TERM;
			bool								BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS;
			boost::optional<bool>				EXECUTION_VENUE;
			tOptDate							EFFECTIVE_DATE;
			tOptDate							END_DATE;
			std::string							DAY_COUNT_CONVENTION;
			tOptCcy								SETTLEMENT_CURRENCY;
			assetType							ASSET_CLASS;
			std::string							SUBASSET_CLASS_FOR_OTHER_COMMODITY;
			std::string							TAXONOMY;
			priceFormingContinuationData		PRICE_FORMING_CONTINUATION_DATA;
			std::string							UNDERLYING_ASSET_1;
			std::string							UNDERLYING_ASSET_2;
			std::string							PRICE_NOTATION_TYPE;
			boost::optional<double>				PRICE_NOTATION;
			std::string							ADDITIONAL_PRICE_NOTATION_TYPE;
			boost::optional<double>				ADDITIONAL_PRICE_NOTATION;
			std::string							NOTIONAL_CURRENCY_1;
			std::string							NOTIONAL_CURRENCY_2;
			tOptNomPlus							ROUNDED_NOTIONAL_AMOUNT_1;
			tOptNomPlus							ROUNDED_NOTIONAL_AMOUNT_2;
			std::string							PAYMENT_FREQUENCY_1;
			std::string							PAYMENT_FREQUENCY_2;
			std::string							RESET_FREQUENCY_1;
			std::string							RESET_FREQUENCY_2;
			bool								EMBEDED_OPTION;
			boost::optional<double>				OPTION_STRIKE_PRICE;
			//boost::optional<optionType>				OPTION_TYPE; // TODO: find a taxonomy
			//boost::optional<optionFamily>			OPTION_FAMILY;
			//boost::optional<ccy>					OPTION_CURRENCY;
			//boost::optional<double>					OPTION_PREMIUM;
			//boost::optional<boost::gregorian::date> OPTION_LOCK_PERIOD;
			//boost::optional<boost::gregorian::date> OPTION_EXPIRATION_DATE;
			//std::string								PRICE_NOTATION_TYPE_2;
			//boost::optional<double>					PRICE_NOTATION_2;
			//std::string								PRICE_NOTATION_TYPE_3;
			//boost::optional<double>					PRICE_NOTATION_3;
		};
	}
}

//const static std::string testString("\"58919739\",\"\",\"NEW\",\"2017-01-10T07:52:46\",\"U\",\"FC\",\"\",\"N\",\"N\",\"OFF\",\"2017-02-01\",\"2017-02-28\",\"\",\"\",\"CO\",\"Energy\",\"Commodity:Energy:Elec:Swap:Cash\",\"Trade\",\"ELECTRICITY-DAILY PHELIX BASE SPOT-EEX\",\"\",\"EUR:MWh\",\"40.9775\",\"\",\"\",\"MWH\",\"MWH\",\"3,400\",\"3,400\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\"");

#endif
