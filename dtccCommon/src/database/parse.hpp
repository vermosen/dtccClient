#ifndef PARSE_HPP_
#define PARSE_HPP_

#include <string>
#include <vector>

#define FUSION_MAX_VECTOR_SIZE 45

#include "records\tradeRecordGrammar.hpp"
#include "records\tradeRecord.hpp"

namespace dtcc
{
	namespace database
	{
		bool parse(std::string::const_iterator iter, std::string::const_iterator end, std::vector<dtcc::database::tradeRecord> & recs);
	}
}

#endif