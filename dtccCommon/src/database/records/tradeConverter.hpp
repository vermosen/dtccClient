#ifndef RECORDS_TRADE_CONVERTER_HPP_
#define RECORDS_TRADE_CONVERTER_HPP_

#include <ctime>

#include <boost/date_time/posix_time/conversion.hpp>

#include <soci/soci.h>

#include "database/records/tradeRecord.hpp"
#include "database/enums/optionType.hpp"
#include "database/enums/optionFamily.hpp"
#include "database/enums/collateralization.hpp"

namespace soci
{
	template <> 
	struct type_conversion <dtcc::database::tTime>
	{
		typedef std::tm base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tTime & mi)
		{
			if (ind == i_null)
				throw soci_error("Null value not allowed for this type");

			mi = boost::posix_time::ptime_from_tm(i);
		}

		static void to_base(const dtcc::database::tTime & mi, base_type & i, indicator & ind)
		{
			i = boost::posix_time::to_tm(mi);
			ind = i_ok;
		}
	};

	template <>
	struct type_conversion <bool>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, bool & mi)
		{
			if (ind == i_null)
				throw soci_error("Null value not allowed for this type");

			// TODO: branch free version
			if (i == 0) mi = false;
			else if (i == 1) mi = true;
			else throw soci_error("data type does not match");
		}

		static void to_base(const bool & mi, base_type & i, indicator & ind)
		{
			i = static_cast<int>(mi);
			ind = i_ok;
		}
	};

	template <>
	struct type_conversion <dtcc::database::optionType>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::optionType & mi)
		{
			if (ind == i_null)
				throw soci_error("Null value not allowed for this type");

			mi = static_cast<dtcc::database::optionType>(i);
		}

		static void to_base(const dtcc::database::optionType & mi, base_type & i, indicator & ind)
		{
			i = static_cast<int>(mi);
			ind = i_ok;
		}
	};

	template <>
	struct type_conversion <dtcc::database::optionFamily>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::optionFamily & mi)
		{
			if (ind == i_null)
				throw soci_error("Null value not allowed for this type");

			mi = static_cast<dtcc::database::optionFamily>(i);
		}

		static void to_base(const dtcc::database::optionFamily & mi, base_type & i, indicator & ind)
		{
			i = static_cast<int>(mi);
			ind = i_ok;
		}
	};

	template <>
	struct type_conversion <dtcc::database::tOptCollat>
	{
		typedef int base_type;

		static void from_base(base_type i, indicator ind, dtcc::database::tOptCollat & mi)
		{
			if (ind == i_null)
				mi = boost::none;
			else
				mi = static_cast<dtcc::database::collateralization>(i);
		}

		static void to_base(const dtcc::database::tOptCollat & mi, base_type & i, indicator & ind)
		{
			if(mi != boost::none)
				i = static_cast<int>(*mi);
			ind = i_null;
		}
	};
}

#endif