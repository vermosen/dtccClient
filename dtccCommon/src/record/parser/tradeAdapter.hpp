#ifndef TRADE_ADAPTER_HPP_
#define TRADE_ADAPTER_HPP_

#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional_io.hpp>

#include "record/tradeRecord.hpp"

typedef boost::tuple<int, int, int, int, int, int> timeAdaptator;
typedef boost::optional<timeAdaptator> optTimeAdaptator;
typedef boost::optional<boost::tuple<int, int, int> > optDateAdaptator;

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::database::tradeRecord,
	(int, DISSEMINATION_ID)
	(boost::optional<int>, ORIGINAL_DISSEMINATION_ID)
	(dtcc::database::action, ACTION)
	(dtcc::database::tTime, EXECUTION_TIMESTAMP)
	(dtcc::database::cleared, CLEARED)
	(dtcc::database::tOptCollat, INDICATION_OF_COLLATERALIZATION)
	(boost::optional<bool>, INDICATION_OF_END_USER_EXCEPTION)
	(bool, INDICATION_OF_OTHER_PRICE_AFFECTING_TERM)
	(boost::optional<bool>, BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS)
	(boost::optional<bool>, EXECUTION_VENUE)
	(dtcc::database::tOptDate, EFFECTIVE_DATE)
	(dtcc::database::tOptDate, END_DATE)
	(std::string, DAY_COUNT_CONVENTION)
	(dtcc::database::tOptCcy, SETTLEMENT_CURRENCY)
	(dtcc::database::assetType, ASSET_CLASS)
	(std::string, SUBASSET_CLASS_FOR_OTHER_COMMODITY)
	(std::string, TAXONOMY)
	(dtcc::database::priceFormingContinuationData, PRICE_FORMING_CONTINUATION_DATA)
	(std::string, UNDERLYING_ASSET_1)
	(std::string, UNDERLYING_ASSET_2)
	(std::string, PRICE_NOTATION_TYPE)
	(boost::optional<double>, PRICE_NOTATION)
	(std::string, ADDITIONAL_PRICE_NOTATION_TYPE)
	(boost::optional<double>, ADDITIONAL_PRICE_NOTATION)
	(std::string, NOTIONAL_CURRENCY_1)
	(std::string, NOTIONAL_CURRENCY_2)
	(dtcc::database::tOptNomPlus, ROUNDED_NOTIONAL_AMOUNT_1)
	(dtcc::database::tOptNomPlus, ROUNDED_NOTIONAL_AMOUNT_2)
	(std::string, PAYMENT_FREQUENCY_1)
	(std::string, PAYMENT_FREQUENCY_2)
	(std::string, RESET_FREQUENCY_1)
	(std::string, RESET_FREQUENCY_2)
	(bool, EMBEDED_OPTION)
	(boost::optional<double>, OPTION_STRIKE_PRICE)
	(dtcc::database::tOptType, OPTION_TYPE)
	(dtcc::database::tOptFamily, OPTION_FAMILY)
	(dtcc::database::tOptCcy, OPTION_CURRENCY)
	(boost::optional<double>, OPTION_PREMIUM)
	(dtcc::database::tOptDate, OPTION_LOCK_PERIOD)
	(dtcc::database::tOptDate, OPTION_EXPIRATION_DATE)
	(std::string, PRICE_NOTATION2_TYPE)
	(boost::optional<double>, PRICE_NOTATION2)
	(std::string, PRICE_NOTATION3_TYPE)
	(boost::optional<double>, PRICE_NOTATION3)
	(boost::gregorian::date, FILE_DATE)
)

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::tOptFamily, std::string, boost::spirit::qi::domain >
{
	typedef std::string type;

	static type pre(dtcc::database::tOptFamily a) { return type(); }

	static void post(dtcc::database::tOptFamily& d, type const& v)
	{
		if (v == "")
			d = boost::none;
		else
			d = dtcc::EnumManager<dtcc::database::optionFamily>::toEnum(v);
	}

	static void fail(dtcc::database::tOptFamily&) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::tOptType, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::tOptType a) { return type(); }

	static void post(dtcc::database::tOptType& d, type const& v)
	{
		if (v == "")
			d = boost::none;
		else
			d = dtcc::EnumManager<dtcc::database::optionType>::toEnum(v);
	}

	static void fail(dtcc::database::tOptType&) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::action, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::action a) { return type(); }

	static void post(dtcc::database::action& d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::action>::toEnum(v);
	}

	static void fail(dtcc::database::action&) {}
};

template<>
struct boost::spirit::traits::transform_attribute<dtcc::database::tOptCollat, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::tOptCollat a) { return type(); }

	static void post(dtcc::database::tOptCollat & d, type const& v)
	{
		if (v == "")
			d = boost::none;
		else
			d = dtcc::EnumManager<dtcc::database::collateralization>::toEnum(v);
	}

	static void fail(dtcc::database::tOptCollat &) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::cleared, char, boost::spirit::qi::domain>
{
	typedef char type;

	static type pre(dtcc::database::cleared a) { return type(); }

	static void post(dtcc::database::cleared& d, type const& v)
	{
		d = static_cast<dtcc::database::cleared>(v == 'C');
	}

	static void fail(dtcc::database::cleared&) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::assetType, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::assetType a) { return type(); }

	static void post(dtcc::database::assetType& d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::assetType>::toEnum(v);
	}

	static void fail(dtcc::database::assetType&) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::priceFormingContinuationData, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::priceFormingContinuationData a) { return type(); }

	static void post(dtcc::database::priceFormingContinuationData& d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::priceFormingContinuationData>::toEnum(v);
	}

	static void fail(dtcc::database::priceFormingContinuationData&) {}
};

template<>
struct boost::spirit::traits::transform_attribute< dtcc::database::tTime, timeAdaptator, boost::spirit::qi::domain>
{
	typedef timeAdaptator type;

	static type pre(dtcc::database::tTime a) { return type(); }

	static void post(dtcc::database::tTime & d, type const& v)
	{
		try
		{
			d = boost::posix_time::ptime(
				boost::gregorian::date(boost::get<0>(v), boost::get<1>(v), boost::get<2>(v)),
				boost::posix_time::time_duration(boost::get<3>(v), boost::get<4>(v), boost::get<5>(v)));
		}
		catch (...)
		{
			int i = 0;
		}
	}

	static void fail(dtcc::database::tTime&) {}
};

template<>
struct boost::spirit::traits::transform_attribute<dtcc::database::tOptDate, optDateAdaptator, boost::spirit::qi::domain>
{
	typedef optDateAdaptator type;

	static type pre(dtcc::database::tOptDate a) { return type(); }

	static void post(dtcc::database::tOptDate& d, type const& v)
	{
		if (v)
		{
			try
			{
				d = boost::gregorian::date(boost::get<0>(*v), boost::get<1>(*v), boost::get<2>(*v));
			}
			catch (...)
			{
				int i = 0;
			}
		}
		else d = boost::none;
	}

	static void fail(dtcc::database::tOptDate&)
	{
		int i = 0;
	}
};

#endif