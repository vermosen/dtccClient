#include "eod.hpp"

namespace dtcc
{
	namespace web
	{
		std::string eod::path() const
		{
			std::stringstream path;
			path.imbue(format_);
			path << "slices/" << "CUMULATIVE_" << asset_.fileStr_ << "_" << date_ << ".zip";
			return path.str();
		}

		boost::gregorian::date eod::date() const
		{
			return date_;
		}

		asset::description eod::asset() const
		{
			return asset_;
		}
	}
}