#include "database/records/tradeRecordGrammar.hpp"

namespace dtcc
{
	namespace database
	{
		bool parse(std::string::const_iterator iter, std::string::const_iterator end, std::vector<dtcc::database::tradeRecord> & recs)
		{
			tradeRecordGrammar<std::string::const_iterator, ascii::blank_type> g;

			return (boost::spirit::qi::phrase_parse(iter, end, g, boost::spirit::ascii::blank, recs) && iter == end);
		}
	}
}