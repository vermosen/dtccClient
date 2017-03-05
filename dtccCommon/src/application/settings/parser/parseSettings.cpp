#include "parseSettings.hpp"

#include <boost/spirit/include/qi.hpp>

namespace dtcc
{
	namespace parser
	{
		namespace ascii = boost::spirit::ascii;

		bool parseSettings(	std::string::const_iterator iter,
							std::string::const_iterator end,
							dtcc::settings2 & settings)
		{
			if (iter == end)
			{
				// the file is empty
				return true;
			}
			else
			{
				// create the grammar
				//settingsGrammar<std::string::const_iterator, ascii::space_type> g;

				//return (boost::spirit::qi::phrase_parse(iter, end, g, ascii::space, settings) && iter == end);

				return true;
			}
		}
	}
}