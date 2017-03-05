#ifndef TRADE_RECORD_GRAMMAR_HPP_
#define TRADE_RECORD_GRAMMAR_HPP_

#define BOOST_SPIRIT_DEBUG

#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/optional.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional_io.hpp>

#include "application/logger.hpp"
#include "record/tradeRecord.hpp"
#include "tradeAdapter.hpp"

// currency amount policy
template <typename T>
struct currencyPolicy : boost::spirit::qi::real_policies<T>
{
	//  No exponent
	template <typename iterator>
	static bool
		parse_exp(iterator&, iterator const&)
	{
		return false;
	}

	//  No exponent
	template <typename iterator, typename Attribute>
	static bool
		parse_exp_n(iterator&, iterator const&, Attribute&)
	{
		return false;
	}

	//  Thousands separated numbers
	template <typename iterator, typename Attribute>
	static bool
		parse_n(iterator& first, iterator const& last, Attribute& attr)
	{
		using boost::spirit::qi::uint_parser;
		namespace qi = boost::spirit::qi;

		uint_parser<unsigned, 10, 1, 3> uint3;
		uint_parser<unsigned, 10, 3, 3> uint3_3;

		T result = 0;
		if (parse(first, last, uint3, result))
		{
			T n; iterator save = first;
			while (qi::parse(first, last, ",") && qi::parse(first, last, uint3_3, n))
			{
				result = result * 1000 + n;
				save = first;
			}

			first = save;
			attr = result;
			return true;
		}
		return false;
	}
};

using namespace boost::spirit;

struct errorHandler 
{
	errorHandler() = default; 
	errorHandler(errorHandler const&) = delete;

	template<typename...> struct result { typedef void type; };
	template<typename Iter> void operator()(
		Iter & first_iter, Iter last_iter,
		Iter error_iter, const qi::info& what) const
	{
		// store the beginning of the record
		auto temp = first_iter;

		if (first_iter != last_iter)
		{
			// skip the record
			while (*(first_iter++) != '\n');

			LOG_WARNING() << std::string("Incoherent record found: ") << std::string(temp, first_iter);
		}
	}
};

template <typename iterator, typename skipper>
struct tradeRecordGrammar : qi::grammar<iterator, std::vector<dtcc::database::tradeRecord>(), skipper>
{
	tradeRecordGrammar(const boost::gregorian::date & fileDate)
		: tradeRecordGrammar::base_type(records), fileDate_(fileDate)
	{
		rInt		
			%= qi::lexeme['"' >> qi::int_ >> '"'];

		rOptInt
			%= "\"\""
				|
			qi::lexeme['"' >> qi::int_ >> '"'];

		rOptString
			%= "\"\""
				|
			qi::lexeme['"' >> *(ascii::char_ - '"') >> '"'];

		rString 
			%= qi::lexeme['"' >> +(ascii::char_ - '"') >> '"'];

		rTime
			%= '"' >>	qi::int_[_pass = (_1 >= 1400	&& _1 < 10000	)] >> "-" >>
						qi::int_[_pass = (_1 > 0		&& _1 <= 12		)] >> "-" >>
						qi::int_[_pass = (_1 > 0		&& _1 <= 31		)] >> 'T' >>
						qi::int_[_pass = (_1 >= 0		&& _1 <= 24		)] >> ":" >>
						qi::int_[_pass = (_1 >= 0		&& _1 <  60		)] >> ":" >>
						qi::int_[_pass = (_1 >= 0		&& _1 <  60		)] >> '"';

		rOptTime 
			%= "\"\""
				|
				'"'	>>	qi::int_[_pass = (_1 >= 1400	&& _1 < 10000	)] >> "-" >>
						qi::int_[_pass = (_1 > 0		&& _1 <= 12		)] >> "-" >>
						qi::int_[_pass = (_1 > 0		&& _1 <= 31		)] >> 'T' >>
						qi::int_[_pass = (_1 >= 0		&& _1 <= 24		)] >> ":" >>
						qi::int_[_pass = (_1 >= 0		&& _1 <  60		)] >> ":" >>
						qi::int_[_pass = (_1 >= 0		&& _1 <  60		)] >> '"';

		rOptDate
			%= "\"\""
				|
				'"' >> qi::int_[_pass = (_1 >= 1400 && _1 < 10000	)] >> "-"
					>> qi::int_[_pass = (_1 > 0		&& _1 <= 12		)] >> "-"
					>> qi::int_[_pass = (_1 > 0		&& _1 <= 31		)] >> '"';
		
		rCleared 
			%= qi::lexeme['"' >> (ascii::char_("CU")) >> '"'];

		rIndOfCollat 
			%= qi::lexeme['"' >> *(ascii::char_("FOPUC")) >> '"'];
		
		rOptBool 
			%= qi::lexeme['"' >> qi::eps > (qi::lit("Y")[_val = true		] |
											qi::lit("N")[_val = false		] |
											qi::lit("" )[_val = boost::none	]) >> '"'];
		rBool 
			%= qi::lexeme['"' >> qi::eps > (qi::lit("Y")[_val = true] |
											qi::lit("N")[_val = false]) >> '"'];

		rOptVenue 
			%= qi::lexeme['"' >> qi::eps > (qi::lit("ON")	[_val = true		] |
											qi::lit("OFF")	[_val = false		] |
											qi::lit("")		[_val = boost::none	]) >> '"'];
		rOptCcy
			%= "\"\""
				|
			qi::lexeme['"' >> (ascii::char_ - '"') >> (ascii::char_ - '"') >> (ascii::char_ - '"') >> '"'];

		rAssetClass
			%= qi::lexeme['"' >> (ascii::char_ - '"') >> (ascii::char_ - '"') >> '"'];

		rOptNom
			%= "\"\""
				|
			qi::lexeme['"' >> pCurrency >> '"'];
		
		rOptNomPlus
			%= "\"\""
				|
			qi::lexeme['"' >> pCurrency >> -ascii::char_("+") >> '"'];

		rEmbedded
			%= "\"\""
				|
			qi::lexeme['"' >> qi::eps > (qi::lit("EMBED1")[_val = true] | qi::lit("")[_val = false]) >> '"'];

		rFileDate
			%= qi::lit("")[_val = fileDate_];

		//rOptCcyPlus.name("rOptCcyPlus");
		//BOOST_SPIRIT_DEBUG_NODE(rOptCcyPlus);
		//debug(rOptCcyPlus);

		record 
			%=  rInt			> ',' >
				rOptInt			> ',' >
				rString			> ',' >
				rTime			> ',' >
				rCleared		> ',' >
				rIndOfCollat	> ',' >
				rOptBool		> ',' >
				rBool			> ',' >
				rOptBool		> ',' >
				rOptVenue		> ',' >
				rOptDate		> ',' >
				rOptDate		> ',' >
				rOptString		> ',' >
				rOptCcy			> ',' >
				rAssetClass		> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rString			> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rOptNom			> ',' >
				rOptString		> ',' >
				rOptNom			> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rOptNomPlus		> ',' >
				rOptNomPlus		> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rEmbedded		> ',' >
				rOptNom			> ',' >
				rOptString		> ',' >
				rOptString		> ',' >
				rOptCcy			> ',' >
				rOptNom			> ',' >
				rOptDate		> ',' >
				rOptDate		> ',' >
				rOptString		> ',' >
				rOptNom			> ',' >
				rOptString		> ',' >
				rOptNom			> 
				rFileDate >> qi::eol;

		records = +(record) >> qi::eoi;

		qi::on_error<qi::retry>
		(
			record, boost::phoenix::bind
			(boost::phoenix::ref(errorHandler_), 
				qi::_1, // it start
				qi::_2, // it end
				qi::_3, // it error
				qi::_4  // error what 
			)
		);
	}

	qi::rule<iterator, int()						, skipper> rInt;
	qi::rule<iterator, boost::optional<int>()		, skipper> rOptInt;
	qi::rule<iterator, timeAdaptator()				, skipper> rTime;
	qi::rule<iterator, optTimeAdaptator()			, skipper> rOptTime;
	qi::rule<iterator, optDateAdaptator()			, skipper> rOptDate;
	qi::rule<iterator, std::string()				, skipper> rString;
	qi::rule<iterator, std::string()				, skipper> rOptString;
	qi::rule<iterator, char()						, skipper> rCleared;
	qi::rule<iterator, std::string()				, skipper> rIndOfCollat;
	qi::rule<iterator, boost::optional<bool>()		, skipper> rOptBool;
	qi::rule<iterator, bool()						, skipper> rBool;
	qi::rule<iterator, boost::optional<bool>()		, skipper> rOptVenue;
	qi::rule<iterator, dtcc::database::tOptCcy()	, skipper> rOptCcy;
	qi::rule<iterator, std::string()				, skipper> rAssetClass;
	qi::rule<iterator, boost::optional<double>()	, skipper> rOptNom;
	qi::rule<iterator, dtcc::database::tOptNomPlus(), skipper> rOptNomPlus;
	qi::rule<iterator, bool()						, skipper> rEmbedded;
	qi::rule<iterator, boost::gregorian::date()		, skipper> rFileDate;

	qi::rule<iterator, dtcc::database::tradeRecord(), skipper> record;
	qi::rule<iterator, std::vector<dtcc::database::tradeRecord>(), skipper> records;

	qi::real_parser<double, currencyPolicy<int> > pCurrency;

	boost::gregorian::date fileDate_;

	errorHandler errorHandler_;
};

#endif
