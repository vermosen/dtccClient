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

		virtual const std::string & url()
		{
			std::stringstream url;
			url.imbue(format_);
			url << host_ << "CUMULATIVE_" << asset_.fileStr_ << "_" << dt_ << ".zip";

			return url.str();
		}

	private:
		int port_;
		std::string host_;
		std::string base_;
		settings::asset asset_;
		boost::gregorian::date dt_;

		static std::locale format_;
	};
}

#endif /* QUERY_EOD_HPP_ */