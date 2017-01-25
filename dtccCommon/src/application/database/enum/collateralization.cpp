#include "application/database/enum/collateralization.hpp"

namespace dtcc
{
	namespace database
	{
		std::ostream & operator << (std::ostream &out, const collateralization&value)
		{
			switch (value)
			{
			case collateralization::fc:
				out << "FC";
				break;
			case collateralization::oc:
				out << "OC";
				break;
			case collateralization::pc:
				out << "PC";
				break;
			case collateralization::uc:
				out << "UC";
				break;
			default:
				out << "<unknown>";
			}

			return out;
		}
	}
}
