#include "parse.hpp"

#include <boost/spirit/include/qi.hpp>

namespace dtcc
{
	namespace parser
	{
		bool parse(	std::string::const_iterator iter, 
					std::string::const_iterator end, 
					std::vector<database::tradeRecord> & recs, 
					const boost::gregorian::date & dt)
		{
			// skip the header
			while (*iter++ != '\n');

			if (iter == end)
			{
				// the file is empty
				return true;
			}
			else
			{
				// create the grammar
				tradeRecordGrammar<std::string::const_iterator, ascii::blank_type> g(dt);

				return (boost::spirit::qi::phrase_parse(iter, end, g, boost::spirit::ascii::blank, recs) && iter == end);
			}
		}
	}
}