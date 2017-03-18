#include "intraday.hpp"

namespace dtcc
{
	std::locale intraday::format_(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));

	std::string intraday::path() const
	{
		std::stringstream path;
		path.imbue(format_);
		path << "slices/" << "SLICE_" << asset_.fileStr_ << "_" << dt_ << "_" << counter_ << ".zip";
		return path.str();
	}
}
