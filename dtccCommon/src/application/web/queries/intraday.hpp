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
			intraday(	const boost::gregorian::date & date,
						const asset::description & asset,
						int counter = 1)
				: query()
				, asset_(asset)
				, date_(date)
				, counter_(counter) {}

			virtual std::string path() const;
			inline boost::gregorian::date date() const;
			inline asset::description asset() const;
			inline int & counter();

		private:
			asset::description asset_;
			boost::gregorian::date date_;
			int counter_;

			static std::locale format_;
		};
	}
}

#endif /* QUERY_EOD_HPP_ */