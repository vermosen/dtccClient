#ifndef QUERY_INTRADAY_HPP_
#define QUERY_INTRADAY_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/web/query.hpp"
#include "application/asset/description.hpp"

namespace dtcc
{
	namespace web
	{
		class intraday : public query
		{
		public:
			intraday(const boost::gregorian::date & dt,
				const asset::description & asset,
				int counter = 1)
				: query()
				, asset_(asset)
				, dt_(dt)
				, counter_(counter) {}

			virtual std::string path() const;

		private:
			asset::description asset_;
			boost::gregorian::date dt_;
			int counter_;

			static std::locale format_;
		};
	}
}

#endif /* QUERY_EOD_HPP_ */