#ifndef QUERY_EOD_HPP_
#define QUERY_EOD_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/query.hpp"
#include "application/settings.hpp"

namespace dtcc
{
	class eod : public query
	{
	public:
		eod(const boost::gregorian::date & dt,
			const settings::asset & asset)
			: query()
			, asset_(asset)
			, dt_(dt) {}

		virtual std::string path() const;

	private:
		settings::asset asset_;
		boost::gregorian::date dt_;
		static std::locale format_;
	};
}

#endif /* QUERY_EOD_HPP_ */