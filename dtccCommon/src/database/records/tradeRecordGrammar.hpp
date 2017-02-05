#ifndef TRADE_RECORD_GRAMMAR_HPP_
#define TRADE_RECORD_GRAMMAR_HPP_

#include <string>

// 3 hours wasted on this one...
#define FUSION_MAX_VECTOR_SIZE 45

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional_io.hpp>

#include "database/records/tradeRecord.hpp"

typedef boost::tuple<int, int, int, int, int, int> timeAdaptator;
typedef boost::optional<boost::tuple<int, int, int> > optDateAdaptator;

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::database::tradeRecord,
	(int, DISSEMINATION_ID)
	(boost::optional<int>, ORIGINAL_DISSEMINATION_ID)
	(dtcc::database::action, ACTION)
	(boost::posix_time::ptime, EXECUTION_TIMESTAMP)
	(dtcc::database::cleared, CLEARED)
	(dtcc::database::collateralization, INDICATION_OF_COLLATERALIZATION)
	(boost::optional<bool>, INDICATION_OF_END_USER_EXCEPTION)
	(bool, INDICATION_OF_OTHER_PRICE_AFFECTING_TERM)
	(bool, BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS)
	(boost::optional<bool>, EXECUTION_VENUE)
	(boost::optional<boost::gregorian::date>, EFFECTIVE_DATE)
	(boost::optional<boost::gregorian::date>, END_DATE)
	/*(std::string, DAY_COUNT_CONVENTION)
	(boost::optional<dtcc::database::tradeRecord::ccy>, SETTLEMENT_CURRENCY)
	(dtcc::database::assetType, ASSET_CLASS)
	(std::string, SUBASSET_CLASS_FOR_OTHER_COMMODITY)
	(std::string, TAXONOMY)
	(dtcc::database::priceFormingContinuationData, PRICE_FORMING_CONTINUATION_DATA)
	(std::string, UNDERLYING_ASSET_1)
	(std::string, UNDERLYING_ASSET_2)
	(std::string, PRICE_NOTATION_TYPE)
	(boost::optional<double>, PRICE_NOTATION)
	(std::string, ADDITIONAL_PRICE_NOTATION_TYPE)
	(std::string, ADDITIONAL_PRICE_NOTATION)
	(std::string, NOTIONAL_CURRENCY_1)
	(std::string, NOTIONAL_CURRENCY_2)
	(boost::optional<double>, ROUNDED_NOTIONAL_AMOUNT_1)
	(bool, ROUNDED_NOTIONAL_AMOUNT_1_PLUS)
	(boost::optional<double>, ROUNDED_NOTIONAL_AMOUNT_2)
	(bool, ROUNDED_NOTIONAL_AMOUNT_2_PLUS)
	(std::string, PAYMENT_FREQUENCY_1)
	(std::string, PAYMENT_FREQUENCY_2)
	(std::string, RESET_FREQUENCY_1)
	(std::string, RESET_FREQUENCY_2)
	(bool, EMBEDED_OPTION)
	(boost::optional<double>, OPTION_STRIKE_PRICE)
	(boost::optional<dtcc::database::optionType>, OPTION_TYPE)
	(boost::optional<dtcc::database::optionFamily>, OPTION_FAMILY)
	(boost::optional<dtcc::database::tradeRecord::ccy>, OPTION_CURRENCY)
	(boost::optional<double>, OPTION_PREMIUM)
	(boost::optional<boost::gregorian::date>, OPTION_LOCK_PERIOD)
	(boost::optional<boost::gregorian::date>, OPTION_EXPIRATION_DATE)
	(std::string, PRICE_NOTATION_TYPE_2)
	(boost::optional<double>, PRICE_NOTATION_2)
	(std::string, PRICE_NOTATION_TYPE_3)
	(boost::optional<double>, PRICE_NOTATION_3)*/
)

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
struct boost::spirit::traits::transform_attribute<dtcc::database::collateralization, std::string, boost::spirit::qi::domain>
{
	typedef std::string type;

	static type pre(dtcc::database::collateralization a) { return type(); }

	static void post(dtcc::database::collateralization & d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::collateralization>::toEnum(v);
	}

	static void fail(dtcc::database::collateralization &) {}
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
struct boost::spirit::traits::transform_attribute< dtcc::database::tTime, timeAdaptator, boost::spirit::qi::domain>
{
	typedef timeAdaptator type;

	static type pre(dtcc::database::tTime a) { return type(); }

	static void post(dtcc::database::tTime& d, type const& v)
	{
		d = dtcc::database::tTime(
				boost::gregorian::date(boost::get<0>(v), boost::get<1>(v), boost::get<2>(v)),
				boost::posix_time::time_duration(boost::get<3>(v), boost::get<4>(v), boost::get<5>(v)));
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
		(v ? d = boost::gregorian::date(boost::get<0>(*v), boost::get<1>(*v), boost::get<2>(*v)) : d = boost::none);
	}

	static void fail(dtcc::database::tOptDate&) {}
};

using namespace boost::spirit;

template <typename Iterator>
struct tradeRecordGrammar : qi::grammar<Iterator, dtcc::database::tradeRecord(), ascii::space_type>
{
	tradeRecordGrammar() : tradeRecordGrammar::base_type(start)
	{
		rInt		
			%= qi::lexeme['"' >> int_ >> '"'];

		rOptInt
			%= ascii::no_case["\"\""]
					|
				qi::lexeme['"' >> (qi::int_) >> '"'];

		rAction 
			%= qi::lexeme['"' >> +(ascii::char_ - '"') >> '"'];

		rTime 
			%= qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> 'T' >>
								qi::int_ >> ":" >> qi::int_ >> ":" >> qi::int_ >> '"'];

		rOptDate 
			%= ascii::no_case["\"\""] 
					| 
				qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> '"'];
		
		rCleared 
			%= qi::lexeme['"' >> (ascii::char_("CU")) >> '"'];

		rIndOfCollat 
			%= qi::lexeme['"' >> *(ascii::char_("FOPUC")) >> '"'];
		
		rOptBool 
			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("Y")[_val = true		] |
														qi::lit("N")[_val = false		] |
														qi::lit("" )[_val = boost::none	])] >> '"'];
		rBool 
			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("Y")[_val = true] |
														qi::lit("N")[_val = false])] >> '"'];

		rOptVenue 
			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("ON")	[_val = true		] |
														qi::lit("OFF")	[_val = false		] |
														qi::lit("")		[_val = boost::none	])] >> '"'];

		start %=
			rInt >> ',' >>
			rOptInt >> ',' >>
			rAction >> ',' >>
			rTime >> ',' >>
			rCleared >> ',' >>
			rIndOfCollat >> ',' >>
			rOptBool >> "," >>
			rBool >> "," >>
			rBool >> "," >>
			rOptVenue >> "," >>
			rOptDate >> ',' >>
			rOptDate
			;
	}

	qi::rule<Iterator, int(), ascii::space_type> rInt;
	qi::rule<Iterator, int(), ascii::space_type> rOptInt;
	qi::rule<Iterator, timeAdaptator(), ascii::space_type> rTime;
	qi::rule<Iterator, optDateAdaptator(), ascii::space_type> rOptDate;
	qi::rule<Iterator, std::string(), ascii::space_type> rAction;
	qi::rule<Iterator, char(), ascii::space_type> rCleared;
	qi::rule<Iterator, std::string(), ascii::space_type> rIndOfCollat;
	qi::rule<Iterator, boost::optional<bool>(), ascii::space_type>	rOptBool;
	qi::rule<Iterator, bool(), ascii::space_type> rBool;
	qi::rule<Iterator, boost::optional<bool>(), ascii::space_type> rOptVenue;

	qi::rule<Iterator, dtcc::database::tradeRecord(), ascii::space_type> start;
};

#endif
