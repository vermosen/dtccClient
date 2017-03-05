#ifndef RECORD_PARSE_HPP_
#define RECORD_PARSE_HPP_

#include <string>
#include <vector>

#define FUSION_MAX_VECTOR_SIZE 45

#include "tradeRecordGrammar.hpp"
#include "record\tradeRecord.hpp"

namespace dtcc
{
	namespace parser
	{
		bool parseRecords(	std::string::const_iterator				iter, 
							std::string::const_iterator				end	, 
							std::vector<database::tradeRecord> &	recs, 
							const boost::gregorian::date &			dt	);
	}
}

#endif