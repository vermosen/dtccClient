#ifndef QUERY_INTRADAY_HPP_
#define QUERY_INTRADAY_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/connection.hpp"
#include "application/settings.hpp"

namespace dtcc
{
	class intraday : public query
	{
	public:
		intraday(	const std::string & host,
					int port,
					const boost::gregorian::date & dt,
					const settings::asset & asset,
					int counter = 1)
			: query(host, port)
			, asset_(asset)
			, dt_(dt)
			, counter_(counter) {}

		virtual const std::string & url()
		{
			std::stringstream url;
			url.imbue(format_);
			url << host_ << "SLICE_" << asset_.fileStr_ << "_" << dt_ << "_" << counter_ << ".zip";
			return url.str();
		}

	private:
		settings::asset asset_;
		boost::gregorian::date dt_;
		int counter_;

		static std::locale format_;
	};
}

#endif /* QUERY_EOD_HPP_ */