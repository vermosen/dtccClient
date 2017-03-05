#ifndef STARTUP_HPP_
#define STARTUP_HPP_

#include <string>
#include <map>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

namespace dtcc
{
	namespace qi = boost::spirit::qi;

	template <typename iterator>
	struct startupParser : qi::grammar<iterator, std::pair<std::string, std::string>()>
	{
		startupParser() : startupParser::base_type(pair)
		{
			pair = "-" >> key >> -('=' >> value);
			key = *(qi::char_ - '=');
			value = +qi::char_;
		}

		qi::rule<iterator, std::pair<std::string, std::string>()> pair;
		qi::rule<iterator, std::string()> key, value;
	};

	std::map<std::string, std::string> parseStartup(int argc, char ** argv);
}
#endif