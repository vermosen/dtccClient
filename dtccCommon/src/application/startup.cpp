#include "startup.hpp"

#include "application/logger.hpp"

namespace dtcc
{
	std::map<std::string, std::string> parseStartup(int argc, char ** argv)
	{
		std::map<std::string, std::string> ret;

		typedef std::string::const_iterator iterator_type;

		startupParser<iterator_type> g;

		for (int i = 1; i < argc; i++)
		{
			std::string s(argv[i]);
			std::pair<std::string, std::string> keyval;
			iterator_type iter = s.cbegin(), end = s.cend();

			if (qi::parse(iter, end, g, keyval) && iter == end)
			{
				if (ret.find(keyval.first) != ret.end())
				{
					LOG_WARNING() << "duplicate key " << keyval.first << " found in startup command";
				}
				else
				{
					ret.insert(keyval);
				}
			}
			else
			{
				LOG_FATAL() << "failed to parse startup argument " << s;
				throw std::exception();
			}
		}

		return ret;
	}
}