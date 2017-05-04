#ifndef SETTINGS_PARSER_HPP_
#define SETTINGS_PARSER_HPP_

#include <string>
#include <vector>

#include "settings/parser/settingsGrammar.hpp"
#include "settings.hpp"

namespace dtcc
{
	namespace parser
	{
		bool parseSettings(	std::string::const_iterator iter,
							std::string::const_iterator end,
							dtcc::settings & settings);
	}
}

#endif
