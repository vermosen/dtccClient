#ifndef MAIN_CPP_
#define MAIN_CPP_

#ifdef _WIN32
#include "StdAfx.h"
#include <Windows.h>
#else
#include <unistd.h>
#endif
	
#include <exception>
#include <boost/thread.hpp>
#include "application/logger.hpp"
#include "application/service.hpp"

class serviceImpl
{
public:

	void loop()
	{
		while (run_)
		{
			LOG_INFO() << "tic";
			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		}
	}

	void onStartImpl(DWORD dwArgc, LPSTR * pszArgv)
	{
		boost::thread * thr = new boost::thread(boost::bind(&serviceImpl::loop, this));
		thr->detach();
	}
	void onStopImpl() { run_ = false; boost::this_thread::sleep(boost::posix_time::milliseconds(5000)); }
	void onPauseImpl() { run_ = false; }
	void onContinueImpl() { run_ = true; }
	void onShutdownImpl() { run_ = false; }

	std::string name() { return "dtccDownload"; };
	bool canStop() { return true; };
	bool canShutdown() { return true; };
	bool canPauseContinue() { return true; };

private:
	bool run_ = true;
};

int main(int * argc, char ** argv)
{
	try
	{
		dtcc::logger::setLogger("dtcc_%Y%m%d_%3N.log", dtcc::logger::Info);
		LOG_INFO() << "Application is starting";

		LOG_INFO() << "Trying to create a new Windows service";
		dtcc::service<serviceImpl> srv;

		if (!dtcc::service<serviceImpl>::run(srv))
		{
			LOG_ERROR() << "service cannot be started";
		}
		else
		{
			LOG_INFO() << "Successfully create a new Windows service";
		}

		LOG_INFO() << "Application is stopping";
		return 0;
	}
	catch (std::exception & e)
	{
		LOG_ERROR() << e.what();
		return 1;
	}
}

#endif