#ifndef RECORD_PARSE_HPP_
#define RECORD_PARSE_HPP_

#include <string>
#include <vector>

#define FUSION_MAX_VECTOR_SIZE 45

#include "tradeRecordGrammar.hpp"
#include "database/record/tradeRecord.hpp"

namespace dtcc
{
	namespace parser
	{
		bool parseRecords(	std::string::iterator					iter, 
							std::string::iterator					end	, 
							std::vector<database::tradeRecord> &	recs, 
							const boost::gregorian::date &			dt	);
	}
}

#endif