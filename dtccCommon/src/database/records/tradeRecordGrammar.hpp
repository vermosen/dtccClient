#ifndef TRADE_RECORD_GRAMMAR_HPP_
#define TRADE_RECORD_GRAMMAR_HPP_

#include <string>

//#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>

#include "database/records/tradeRecord.hpp"

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::database::tradeRecord,
	(int64_t, DISSEMINATION_ID)
	(boost::optional<int64_t>, ORIGINAL_DISSEMINATION_ID)
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
struct boost::spirit::traits::transform_attribute< boost::posix_time::ptime, 
	std::tuple<int, int, int, int, int, int>,
	boost::spirit::qi::domain>
{
	typedef std::tuple<int, int, int, int, int, int> type;

	static type pre(boost::posix_time::ptime a) { return type(); }

	static void post(boost::posix_time::ptime& d, type const& v)
	{
		d = boost::posix_time::ptime(
				boost::gregorian::date(std::get<0>(v), std::get<1>(v), std::get<2>(v)),
				boost::posix_time::time_duration(std::get<3>(v), std::get<4>(v), std::get<5>(v)));
	}

	static void fail(boost::posix_time::ptime&) {}
};

template<>
struct boost::spirit::traits::transform_attribute<boost::optional<boost::gregorian::date>,
	boost::optional<std::tuple<int, int, int> >,
	boost::spirit::qi::domain>
{
	typedef boost::optional<std::tuple<int, int, int> > type;

	static type pre(boost::optional<boost::gregorian::date> a) { return type(); }

	static void post(boost::optional<boost::gregorian::date>& d, type const& v)
	{
		(v ? d = boost::gregorian::date(std::get<0>(*v), std::get<1>(*v), std::get<2>(*v)) : d = boost::none);
	}

	static void fail(boost::optional<boost::gregorian::date>&) {}
};

namespace dtcc
{
	namespace database
	{
		using namespace boost::spirit;

		template <typename Iterator>
		struct tradeRecordGrammar : qi::grammar<Iterator, tradeRecord(), ascii::space_type>
		{
			tradeRecordGrammar() : tradeRecordGrammar::base_type(start)
			{
				string			%= qi::lexeme['"' >> +(ascii::char_ - '"') >> '"'];
				int64			%= qi::lexeme['"' >> qi::int_ >> '"'];
				optInt64		%= qi::lexeme['"' >> -(qi::int_) >> '"'];
				action			%= qi::lexeme['"' >> +(ascii::char_ - '"') >> '"'];
				ptime			%= qi::lexeme['"' >> qi::int_ >> "-" >> qi::int_ >> "-" >> qi::int_ >> "T" >> qi::int_ >> ":" >> qi::int_ >> ":" >> qi::int_ >> '"'];
				clr				%= qi::lexeme['"' >> (ascii::char_("CU")) >> '"'];
				indOfCollat		%= qi::lexeme['"' >> -(ascii::char_("FOPUC")) >> '"'];
				optBool			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("Y")[_val = true] | 
																			qi::lit("N")[_val = false] | 
																			qi::lit("")[_val = boost::none])] >> '"'];

				rBool			%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("Y")[_val = true] |
																			qi::lit("N")[_val = false])] >> '"'];

				optVenue		%= qi::lexeme['"' >> qi::no_case[qi::eps > (qi::lit("ON")[_val = true] |
																			qi::lit("OFF")[_val = false] |
																			qi::lit("")[_val = boost::none])] >> '"'];	

				optDate
				
				start %=
					int64		>> "," >>
					optInt64	>> "," >>
					action		>> "," >>
					ptime		>> "," >>
					clr			>> "," >>
					indOfCollat >> "," >> 
					optBool		>> "," >> 
					rBool		>> "," >>
					rBool		>> "," >> 
					optVenue	>> "," >> 
					optDate		>> "," >>
					optDate;
			}

			qi::rule<Iterator, std::string(), ascii::space_type>			string;
			qi::rule<Iterator, int64_t()	, ascii::space_type>			int64;
			qi::rule<Iterator, int64_t()	, ascii::space_type>			optInt64;
			qi::rule<Iterator, std::string(), ascii::space_type>			action;
			qi::rule<Iterator, std::tuple<int, int, int, int, int, int>(), ascii::space_type> ptime;
			qi::rule<Iterator, char()		, ascii::space_type>			clr;
			qi::rule<Iterator, std::string(), ascii::space_type>			indOfCollat;
			qi::rule<Iterator, boost::optional<bool>(), ascii::space_type>	optBool;
			qi::rule<Iterator, bool(), ascii::space_type>					rBool;
			qi::rule<Iterator, boost::optional<bool>(), ascii::space_type>	optVenue;
			qi::rule<Iterator, boost::optional<std::tuple<int, int, int> >(), ascii::space_type>	optDate;

			qi::rule<Iterator, tradeRecord(), ascii::space_type> start;
		};
	}
}

#endif
