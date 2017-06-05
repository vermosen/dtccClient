#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <fstream>
#include <exception>

#include <boost/thread.hpp>

#include "application/serviceImpl.hpp"
#include "application/logger.hpp"
#include "application/startup.hpp"

#include "settings/parser/parseSettings.hpp"
#include "settings.hpp"

#include "utils/debugger.hpp"

#ifdef _WIN32
#include "StdAfx.hpp"
#include <WinSock.h>
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char ** argv)
{
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

		dtcc::logger::initialize(settings.logger_.fileStr_, settings.logger_.severity_);

		LOG_INFO() << "Trying to create a new Windows service...";

		dtcc::serviceImpl srv(settings);

		if (!dtcc::serviceImpl::run(srv))
		{
			LOG_ERROR() << "service cannot be started !";
		}

		LOG_INFO() << "Application is stopping...";
		return 0;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what();
		return 1;
	}
}

#endif