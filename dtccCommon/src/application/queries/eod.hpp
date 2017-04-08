#ifndef QUERY_EOD_HPP_
#define QUERY_EOD_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/query.hpp"
#include "application/asset/description.hpp"

namespace dtcc
{
	class eod : public query
	{
	public:
		eod(const boost::gregorian::date & dt,
			const asset::description & asset)
			: query()
			, asset_(asset)
			, dt_(dt) {}

		virtual std::string path() const;

	private:
		asset::description asset_;
		boost::gregorian::date dt_;
		static std::locale format_;
	};
}

#endif /* QUERY_EOD_HPP_ */