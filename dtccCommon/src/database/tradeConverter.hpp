#ifndef RECORDS_TRADE_CONVERTER_HPP_
#define RECORDS_TRADE_CONVERTER_HPP_

#include <ctime>

#include <boost/date_time/posix_time/conversion.hpp>

#include <soci/soci.h>

#include "record/tradeRecord.hpp"

namespace soci
{
	template <> 
	struct type_conversion <dtcc::database::tOptTime>
	{
		typedef std::tm base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tOptTime & mi)
		{
			if (ind == i_null)
				mi = boost::none;
			else
				mi = boost::posix_time::ptime_from_tm(i);
		}

		static void to_base(const dtcc::database::tOptTime & mi, base_type & i, indicator & ind)
		{
			if (mi == boost::none)
				ind = i_null;
			else
			{
				i = boost::posix_time::to_tm(*mi);
				ind = i_ok;
			}
		}
	};

	template <>
	struct type_conversion <dtcc::database::tTime>
	{
		typedef std::tm base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tTime & mi)
		{
			if (ind == i_null)
				throw soci_error("Null value not allowed for this type");
			else
				mi = boost::posix_time::ptime_from_tm(i);
		}

		static void to_base(const dtcc::database::tTime & mi, base_type & i, indicator & ind)
		{
			i = boost::posix_time::to_tm(mi);
			ind = i_ok;
		}
	};

	template <>
	struct type_conversion <bool>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, bool & mi)
		{
			if (ind == i_null)
				throw soci_error("Null value not allowed for this type");

			// TODO: branch free version
			if (i == 0) mi = false;
			else if (i == 1) mi = true;
			else throw soci_error("data type does not match");
		}

		static void to_base(const bool & mi, base_type & i, indicator & ind)
		{
			i = static_cast<int>(mi);
			ind = i_ok;
		}
	};

	template <>
	struct type_conversion <dtcc::database::tOptType>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tOptType & mi)
		{
			if (ind == i_null)
				mi = boost::none;
			else
				mi = static_cast<dtcc::database::optionType>(i);
		}

		static void to_base(const dtcc::database::tOptType & mi, base_type & i, indicator & ind)
		{
			if (mi == boost::none)
				ind = i_null;
			else
			{
				i = static_cast<int>(*mi);
				ind = i_ok;
			}
		}
	};

	template <>
	struct type_conversion <dtcc::database::tOptFamily>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tOptFamily & mi)
		{
			if (ind == i_null)
				mi = boost::none;
			else
				mi = static_cast<dtcc::database::optionFamily>(i);
		}

		static void to_base(const dtcc::database::tOptFamily & mi, base_type & i, indicator & ind)
		{
			if (mi == boost::none)
				ind = i_null;
			else
			{
				i = static_cast<int>(*mi);
				ind = i_ok;
			}
		}
	};

	template <>
	struct type_conversion <dtcc::database::tOptCollat>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tOptCollat & mi)
		{
			if (ind == i_null)
				mi = boost::none;
			else
				mi = static_cast<dtcc::database::collateralization>(i);
		}

		static void to_base(const dtcc::database::tOptCollat & mi, base_type & i, indicator & ind)
		{
			if(mi != boost::none)
				i = static_cast<int>(*mi);
			else
				ind = i_null;
		}
	};

	template<>
	struct type_conversion<dtcc::database::tradeRecord>
	{
		typedef values base_type;

		static void from_base(values const & v, indicator ind, dtcc::database::tradeRecord & p)
		{
			throw soci_error("not implemented");
			//p.id = v.get<int>("ID");
			//p.firstName = v.get<std::string>("FIRST_NAME");
			//p.lastName = v.get<std::string>("LAST_NAME");
			// p.gender will be set to the default value "unknown"
			// when the column is null:
			//p.gender = v.get<std::string>("GENDER", "unknown");
			// alternatively, the indicator can be tested directly:
			// if (v.indicator("GENDER") == i_null)
			// {
			//     p.gender = "unknown";
			// }
			// else
			// {
			//     p.gender = v.get<std::string>("GENDER");
			// }
		}

		static void to_base(const dtcc::database::tradeRecord & r, values & v, indicator & ind)
		{
			v.set("DISSEMINATION_ID", r.DISSEMINATION_ID);
			v.set("ORIGINAL_DISSEMINATION_ID", r.ORIGINAL_DISSEMINATION_ID, r.ORIGINAL_DISSEMINATION_ID == boost::none ? i_null : i_ok);
			v.set("ACTION", static_cast<int>(r.ACTION));
			v.set("EXECUTION_TIMESTAMP", r.EXECUTION_TIMESTAMP);
			v.set("CLEARED", static_cast<int>(r.CLEARED));
			v.set("INDICATION_OF_COLLATERALIZATION", r.INDICATION_OF_COLLATERALIZATION);
			v.set("INDICATION_OF_END_USER_EXCEPTION", r.INDICATION_OF_END_USER_EXCEPTION);
			v.set("INDICATION_OF_OTHER_PRICE_AFFECTING_TERM", r.INDICATION_OF_OTHER_PRICE_AFFECTING_TERM);
			v.set("BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS", r.BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS);
			v.set("EXECUTION_VENUE", r.EXECUTION_VENUE);
			v.set("EFFECTIVE_DATE", r.EFFECTIVE_DATE);
			v.set("END_DATE", r.END_DATE);
			v.set("DAY_COUNT_CONVENTION", r.DAY_COUNT_CONVENTION);
			v.set("SETTLEMENT_CURRENCY", r.SETTLEMENT_CURRENCY);
			v.set("ASSET_CLASS", static_cast<int>(r.ASSET_CLASS));
			v.set("SUBASSET_CLASS_FOR_OTHER_COMMODITY", r.SUBASSET_CLASS_FOR_OTHER_COMMODITY);
			v.set("TAXONOMY", r.TAXONOMY);
			v.set("PRICE_FORMING_CONTINUATION_DATA", static_cast<int>(r.PRICE_FORMING_CONTINUATION_DATA));
			v.set("UNDERLYING_ASSET_1", r.UNDERLYING_ASSET_1);
			v.set("UNDERLYING_ASSET_2", r.UNDERLYING_ASSET_2);
			v.set("PRICE_NOTATION_TYPE", r.PRICE_NOTATION_TYPE);
			v.set("PRICE_NOTATION", r.PRICE_NOTATION);
			v.set("ADDITIONAL_PRICE_NOTATION_TYPE", r.ADDITIONAL_PRICE_NOTATION_TYPE);
			v.set("ADDITIONAL_PRICE_NOTATION", r.ADDITIONAL_PRICE_NOTATION);
			v.set("NOTIONAL_CURRENCY_1", r.NOTIONAL_CURRENCY_1);
			v.set("NOTIONAL_CURRENCY_2", r.NOTIONAL_CURRENCY_2);
			v.set("ROUNDED_NOTIONAL_AMOUNT_1", r.roundedNotionalAmount1());
			v.set("ROUNDED_NOTIONAL_AMOUNT_1_PLUS", r.roundedNotionalAmountPlus1());
			v.set("ROUNDED_NOTIONAL_AMOUNT_2", r.roundedNotionalAmount2());
			v.set("ROUNDED_NOTIONAL_AMOUNT_2_PLUS", r.roundedNotionalAmountPlus2());
			v.set("PAYMENT_FREQUENCY_1", r.PAYMENT_FREQUENCY_1);
			v.set("PAYMENT_FREQUENCY_2", r.PAYMENT_FREQUENCY_2);
			v.set("RESET_FREQUENCY_1", r.RESET_FREQUENCY_1);
			v.set("RESET_FREQUENCY_2", r.RESET_FREQUENCY_2);
			v.set("EMBEDED_OPTION", r.EMBEDED_OPTION);
			v.set("OPTION_STRIKE_PRICE", r.OPTION_STRIKE_PRICE);
			v.set("OPTION_TYPE", r.OPTION_TYPE);
			v.set("OPTION_FAMILY", r.OPTION_FAMILY);
			v.set("OPTION_CURRENCY", r.OPTION_CURRENCY);
			v.set("OPTION_PREMIUM", r.OPTION_PREMIUM);
			v.set("OPTION_LOCK_PERIOD", r.OPTION_LOCK_PERIOD);
			v.set("OPTION_EXPIRATION_DATE", r.OPTION_EXPIRATION_DATE);
			v.set("PRICE_NOTATION2_TYPE", r.PRICE_NOTATION2_TYPE);
			v.set("PRICE_NOTATION2", r.PRICE_NOTATION2);
			v.set("PRICE_NOTATION3_TYPE", r.PRICE_NOTATION3_TYPE);
			v.set("PRICE_NOTATION3", r.PRICE_NOTATION3);
			v.set("FILE_DATE", r.FILE_DATE);
			ind = i_ok;
		}
	};
}

#endif