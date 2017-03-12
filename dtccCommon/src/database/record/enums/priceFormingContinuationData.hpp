#ifndef ENUM_PRICE_FORMING_CONTINUATION_DATA_HPP_
#define ENUM_PRICE_FORMING_CONTINUATION_DATA_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class priceFormingContinuationData : char
		{
			  amendment = 1
			, increase = 2
			, novation = 3
			, partialTermination = 4
			, termination = 5
			, trade = 6
		};
	}

	template <>
	const std::vector<std::pair<database::priceFormingContinuationData, std::string> >
		enumConversions<database::priceFormingContinuationData>::enumToStringVector =
	{
		 { database::priceFormingContinuationData::amendment			, "Amendment"			}
		,{ database::priceFormingContinuationData::increase				, "Increase"			}
		,{ database::priceFormingContinuationData::novation				, "Novation"			}
		,{ database::priceFormingContinuationData::partialTermination	, "Partialtermination"	}
		,{ database::priceFormingContinuationData::termination			, "Termination"			}
		,{ database::priceFormingContinuationData::trade				, "Trade"				}
	};
}

#endif
