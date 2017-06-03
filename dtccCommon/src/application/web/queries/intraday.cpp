#include "intraday.hpp"

namespace dtcc
{
	namespace web
	{
		std::locale intraday::format_(std::locale::classic(), new boost::gregorian::date_facet("%Y_%m_%d"));

		std::string intraday::path() const
		{
			std::stringstream path;
			path.imbue(format_);
			path << "slices/" << "SLICE_" << asset_.fileStr_ << "_" << date_ << "_" << counter_ << ".zip";
			return path.str();
		}

		boost::gregorian::date intraday::date() const
		{
			return date_;
		}

		asset::description intraday::asset() const
		{
			return asset_;
		}

		int & intraday::counter()
		{ 
			return counter_;
		}
	}
}
