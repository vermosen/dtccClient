#include "application/database/enum/assetType.hpp"

namespace dtcc
{
	namespace database
	{
		std::ostream & operator << (std::ostream &out, const assetType&value)
		{
			switch (value)
			{
			case assetType::rate:
				out << "IR";
				break;
			case assetType::commodity:
				out << "CO";
				break;
			case assetType::equity:
				out << "EQ";
				break;
			case assetType::currency:
				out << "FX";
				break;
			case assetType::credit:
				out << "CR";
				break;
			default:
				out << "<unknown>";
			}

			return out;
		}
	}
}
