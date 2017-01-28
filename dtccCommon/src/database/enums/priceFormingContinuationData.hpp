#ifndef ENUM_PRICE_FORMING_CONTINUATION_DATA_HPP_
#define ENUM_PRICE_FORMING_CONTINUATION_DATA_HPP_

#include "pattern/enumManager.hpp"

namespace dtcc
{
	namespace database
	{
		enum class priceFormingContinuationData
		{
			  amendment
			, increase
			, novation
			, partialTermination
			, termination
			, trade
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
