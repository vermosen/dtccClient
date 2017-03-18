#include "eod.hpp"

namespace dtcc
{
	std::locale eod::format_(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));

	std::string eod::path() const
	{
		std::stringstream path;
		path.imbue(format_);
		path << "slices/" << "CUMULATIVE_" << asset_.fileStr_ << "_" << dt_ << ".zip";

		return path.str();
	}
}