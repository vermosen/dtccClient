#ifndef MAIN_CPP_
#define MAIN_CPP_

//#define FUSION_MAX_VECTOR_SIZE 45

//#include <fstream>
//#include <streambuf>
//#include <exception>
//#include <array>
//#include <vector>
//#include <functional>

//#include <boost/thread.hpp>
//#include <boost/chrono.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/iostreams/device/file.hpp>
//#include <boost/iostreams/stream.hpp>
//#include <boost/lambda/lambda.hpp>

#include "application/logger.hpp"
#include "application/startup.hpp"
#include "application/appRunner.hpp"

#include "settings/parser/parseSettings.hpp"
#include "settings.hpp"

#ifdef _WIN32
#include "StdAfx.hpp"
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char ** argv)
{
	int ret = 1;

	try
	{
		auto args = dtcc::parseStartup(argc, argv);
		
		if (args.find("settings") == args.cend())
		{
			std::cerr << "setting file path cannot be found";
			return 1;
		}

		std::ifstream file(args["settings"], std::ios::in | std::ios::binary);
		std::stringstream buffer; std::string raw; dtcc::settings settings;

		if (file.is_open())
		{
			buffer << file.rdbuf();
			raw = buffer.str();
			
			if (!dtcc::parser::parseSettings(raw.cbegin(), raw.cend(), settings))
			{
				std::cerr << "failed to decode settings";
				return 1;
			}
		}

		// create the logger
		dtcc::logger::initialize(settings.logger_.fileStr_, settings.logger_.severity_);
		LOG_INFO() << "Application is starting...";

		// create the handler object
		dtcc::appRunner app(settings);
		app.run();

		LOG_INFO() << "Application is stopping";
		ret = 0;
	}
	catch (std::exception & e)
	{
		LOG_ERROR() << e.what();
		ret = 1;
	}

	system("pause");
	return ret;
}
#endif
