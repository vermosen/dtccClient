#ifndef SETTINGS_GRAMMAR_HPP_
#define SETTINGS_GRAMMAR_HPP_

#ifdef _DEBUG
#define BOOST_SPIRIT_DEBUG
#endif

#include <iostream>
#include <string>
#include <complex>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>

#include "application/settings/parser/settingsAdapter.hpp"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

template <typename iterator, typename skipper>
struct settingsGrammar : qi::grammar<iterator, dtcc::settings2(), skipper>
{
	settingsGrammar() : settingsGrammar::base_type(start)
	{
		using qi::lit;
		using qi::lexeme;
		using qi::raw;
		using qi::int_;
		using ascii::char_;
		using ascii::string;
		using namespace qi::labels;

		using phx::at_c;
		using phx::push_back;

		rTextBase =
			*(qi::char_ - "</")
			;

		rIntBase =
			lexeme[int_[_val = _1]]
			;

		rDateBase %=
			qi::int_[_pass = (qi::_1 >= 1400 && qi::_1 < 10000)][at_c<0>(_val) = _1]
			> '-'
			> qi::int_[_pass = (qi::_1 > 0 && qi::_1 <= 12)][at_c<1>(_val) = _1]
			> '-'
			> qi::int_[_pass = (qi::_1 > 0 && qi::_1 <= 31)][at_c<2>(_val) = _1]
			;

		rHeader %=
			"<?"
			>> !lit('/')
			>> lexeme[+(char_ - "?>")]
			>> "?>"
			;

		rStartTag =
			'<'
			>> !lit('/')
			>> qi::lit(_r1)[_val = _r1]
			>> '>'
			;

		rEndTag =
			"</"
			>> string(_r1)
			>> '>'
			;

		rText =
			rStartTag(_r1)[_a = _1]
			> rTextBase[_val = _1]
			> rEndTag(_a)
			;

		rInt =
			rStartTag(_r1)[_a = _1]
			>> rIntBase[_val = _1]
			>> rEndTag(_a)
			;

		rDate =
			rStartTag(_r1)[_a = _1]
			> rDateBase[_val = _1]
			> rEndTag(_a)
			;

		rAsset =
			qi::omit[rStartTag(_r1)[_a = _1]]
			> rTextBase
			> qi::omit[rEndTag(_a)]
			;

		rAssets =
			qi::omit[rStartTag(_r1)[_a = _1]]
			> *(rAsset(std::string("asset")))
			> qi::omit[rEndTag(_a)]
			;

		rSettings =
			qi::omit[rStartTag(_r1)[_a = _1]]
			>> rDate(std::string("startDate"))
			>> rDate(std::string("endDate"))
			>> rText(std::string("baseUrl"))
			>> rInt(std::string("cacheSize"))
			>> rAssets(std::string("assets"))
			>> qi::omit[rEndTag(_a)]
			;

		start =
			qi::omit[rHeader] >>
			rSettings(std::string("settings"))[_val = _1]
			;

		start.name("start");
		BOOST_SPIRIT_DEBUG_NODE(start);

	}

	qi::rule<iterator, dtcc::settings2(), ascii::space_type> start;

	qi::rule<iterator, dtcc::settings2(std::string), qi::locals<std::string>, ascii::space_type> rSettings;
	qi::rule<iterator, std::vector<std::string>(std::string), qi::locals<std::string>, ascii::space_type> rAssets;
	qi::rule<iterator, std::string(std::string), qi::locals<std::string>, ascii::space_type> rAsset;
	qi::rule<iterator, dateAdaptator(std::string), qi::locals<std::string>, ascii::space_type> rDate;
	qi::rule<iterator, std::string(std::string), qi::locals<std::string>, ascii::space_type> rText;
	qi::rule<iterator, int(std::string), qi::locals<std::string>, ascii::space_type> rInt;

	qi::rule<iterator, dateAdaptator(), ascii::space_type> rDateBase;
	qi::rule<iterator, std::string(), ascii::space_type> rTextBase;
	qi::rule<iterator, int(), ascii::space_type> rIntBase;

	qi::rule<iterator, std::string(), ascii::space_type> rHeader;
	qi::rule<iterator, std::string(std::string), ascii::space_type> rStartTag;
	qi::rule<iterator, void(std::string), ascii::space_type> rEndTag;
};

#endif
