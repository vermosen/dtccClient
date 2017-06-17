#include "tradeRecord.hpp"

namespace dtcc
{
	namespace database
	{
		boost::optional<double> tradeRecord::roundedNotionalAmount1() const
		{
			if (ROUNDED_NOTIONAL_AMOUNT_1 == boost::none)
				return boost::none;
			else
				return ROUNDED_NOTIONAL_AMOUNT_1->get<0>();
		}

		boost::optional<bool> tradeRecord::roundedNotionalAmountPlus1() const
		{
			if (ROUNDED_NOTIONAL_AMOUNT_1 == boost::none)
				return boost::none;
			else
				return ROUNDED_NOTIONAL_AMOUNT_1->get<1>();
		}

		boost::optional<double> tradeRecord::roundedNotionalAmount2() const
		{
			if (ROUNDED_NOTIONAL_AMOUNT_2 == boost::none)
				return boost::none;
			else
				return ROUNDED_NOTIONAL_AMOUNT_2->get<0>();
		}

		boost::optional<bool> tradeRecord::roundedNotionalAmountPlus2() const
		{
			if (ROUNDED_NOTIONAL_AMOUNT_2 == boost::none)
				return boost::none;
			else
				return ROUNDED_NOTIONAL_AMOUNT_2->get<1>();
		}
	}
}