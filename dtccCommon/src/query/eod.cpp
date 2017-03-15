#include "eod.hpp"

namespace dtcc
{
	std::locale eod::format_(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));
}