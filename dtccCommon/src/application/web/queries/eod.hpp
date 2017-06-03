#ifndef QUERY_EOD_HPP_
#define QUERY_EOD_HPP_

#include <string>
#include <locale>

#include <boost/date_time.hpp>

#include "application/web/query.hpp"
#include "application/asset/description.hpp"

namespace dtcc 
{
	namespace web
	{
		class eod : public query
		{
		public:
			eod(const boost::gregorian::date & date,
				const asset::description & asset)
				: query()
				, asset_(asset)
				, date_(date) {}

			virtual std::string path() const;
			boost::gregorian::date date() const;
			asset::description asset() const;

		private:
			asset::description asset_;
			boost::gregorian::date date_;
			static std::locale format_;
		};
	}
}

#endif /* QUERY_EOD_HPP_ */