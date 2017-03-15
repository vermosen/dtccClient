#include "intraday.hpp"

namespace dtcc
{
	std::locale intraday::format_(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));
}
