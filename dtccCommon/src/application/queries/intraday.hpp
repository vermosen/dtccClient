#ifndef QUERY_INTRADAY_HPP_
#define QUERY_INTRADAY_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/query.hpp"
#include "application/settings.hpp"

namespace dtcc
{
	class intraday : public query
	{
	public:
		intraday(	const boost::gregorian::date & dt,
					const settings::asset & asset,
					int counter = 1)
			: asset_(asset)
			, dt_(dt)
			, counter_(counter) {}

		virtual std::string path() const;

	private:
		settings::asset asset_;
		boost::gregorian::date dt_;
		int counter_;

		static std::locale format_;
	};
}

#endif /* QUERY_EOD_HPP_ */