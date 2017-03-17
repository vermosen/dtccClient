#ifndef QUERY_EOD_HPP_
#define QUERY_EOD_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/connection.hpp"
#include "application/settings.hpp"

namespace dtcc
{
	class eod : public query
	{
	public:
		eod(const std::string & host,
			int port,
			const boost::gregorian::date & dt,
			const settings::asset & asset)
			: query(host, port)
			, asset_(asset)
			, dt_(dt) {}

		virtual std::string path() const
		{
			std::stringstream path;
			path.imbue(format_);
			path << "/slices/" << "CUMULATIVE_" << asset_.fileStr_ << "_" << dt_ << ".zip";

			return path.str();
		}

	private:
		settings::asset asset_;
		boost::gregorian::date dt_;

		static std::locale format_;
	};
}

#endif /* QUERY_EOD_HPP_ */