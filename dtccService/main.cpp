#ifndef MAIN_CPP_
#define MAIN_CPP_
	
#include <fstream>
#include <exception>

#include <boost/thread.hpp>

#include "application/logger.hpp"
#include "application/serviceImpl.hpp"
#include "application/startup.hpp"

#include "settings/parser/parseSettings.hpp"
#include "settings.hpp"

#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif

bool launchDebugger()
{
	// Get System directory, typically c:\windows\system32
	std::wstring systemDir(MAX_PATH + 1, '\0');
	UINT nChars = GetSystemDirectoryW(&systemDir[0], systemDir.length());
	if (nChars == 0) return false; // failed to get system directory
	systemDir.resize(nChars);

	// Get process ID and create the command line
	DWORD pid = GetCurrentProcessId();
	std::wostringstream s;
	s << systemDir << L"\\vsjitdebugger.exe -p " << pid;
	std::wstring cmdLine = s.str();

	// Start debugger process
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) return false;

	// Close debugger process handles to eliminate resource leak
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	// Wait for the debugger to attach
	while (!IsDebuggerPresent()) Sleep(100);

	// Stop execution so the debugger can take over
	DebugBreak();
	return true;
}

int main(int argc, char ** argv)
{
	try
	{
		launchDebugger();

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
		dtcc::serviceImpl srv;

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