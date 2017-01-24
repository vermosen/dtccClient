#include "application/database/record.hpp"
#include "pattern/abstractFactory.hpp"

//#pragma init_seg(compiler)
//template<>
//dtcc::abstractFactory <dtcc::database::record::action, std::string>::map_type * 
//	dtcc::factory <dtcc::database::record::action, std::string>::map_ = new factory::map_type();

namespace dtcc
{
	namespace database
	{
		record::record(const std::string & line)
		{
			std::string token;
			std::stringstream ss(line);

			std::getline(ss, token, ',');
			DISSEMINATION_ID = boost::lexical_cast<int64_t>(token.substr(1, token.size() - 2));

			std::getline(ss, token, ',');
			if (token != "\"\"")
				ORIGINAL_DISSEMINATION_ID = boost::lexical_cast<int64_t>(token.substr(1, token.size() - 2));
			else
				ORIGINAL_DISSEMINATION_ID = boost::none;

			//ACTION =
		}
	}
}