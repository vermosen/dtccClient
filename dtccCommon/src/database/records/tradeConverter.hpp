#ifndef RECORDS_TRADE_CONVERTER_HPP_
#define RECORDS_TRADE_CONVERTER_HPP_

#include <boost/date_time/posix_time/posix_time.hpp>

#include "database/records/tradeRecord.hpp"

namespace dtcc
{
	namespace database
	{
		template<>
		struct type_conversion<tTime>
		{
			typedef int base_type;
			static void from_base(int i, indicator ind, tTime & mi)
			{
				if (ind == i_null)
					throw soci_error("Null value not allowed for this type");

				mi.set(i);
			}

			static void to_base(const tTime & mi, std::tm & i, indicator & ind)
			{
				i = boost::posix_time::ptime::to_tm(mi);
				ind = i_ok;
			}
		};
	}
}

#endif