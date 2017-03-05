#ifndef SETTINGS_GRAMMAR_HPP_
#define SETTINGS_GRAMMAR_HPP_

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

#include "settingsAdapter.hpp"

using namespace boost::spirit;

template <typename iterator, typename skipper>
struct settingsGrammar : qi::grammar<iterator, dtcc::settings2(), skipper>
{
	settingsGrammar()
		: settingsGrammar::base_type(file)
	{
		rText 
			%= qi::lexeme[+(qi::char_ - '<')];

		rStart 
			%= '<' >> !qi::lit('/') > qi::lexeme[+(qi::char_ - '>')] > '>';

		rEnd
			%= "</" > ascii::string(qi::_r1) > '>';

		rHeader
			%= '<?' >> rText >> '?>';

		rInt
			%= rStart[qi::_a = qi::_1] > rText > rEnd(qi::_a);

		settings
			%= rStart[qi::_a = qi::_1] > rInt > rEnd(qi::_a);

		file
			%= qi::omit[rHeader] >> settings >> qi::eoi;
	}

	qi::rule<iterator, std::string(), skipper> rText;
	qi::rule<iterator, std::string(), skipper> rStart;
	qi::rule<iterator, void(std::string), skipper> rEnd;
	qi::rule<iterator, std::string(), skipper> rHeader;
	qi::rule<iterator, int(), skipper> rInt;
	qi::rule<iterator, int(), skipper> settings;
	qi::rule<iterator, dtcc::settings2(), skipper> file;
};

#endif
