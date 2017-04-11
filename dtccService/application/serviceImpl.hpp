#pragma once
#ifndef SERVICE_IMPL_HPP_
#define SERVICE_IMPL_HPP_

#include <boost/thread.hpp>

#include "application/logger.hpp"
#include "application/service.hpp"

namespace dtcc
{
	class serviceImpl : public service<serviceImpl>
	{
	public:

		void serviceMain()
		{
			while (run_)
			{
				LOG_INFO() << "tic";
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			}
		}

		void onStartImpl(DWORD dwArgc, LPSTR * pszArgv)
		{
			LOG_INFO() << "starting service " << name();
			boost::thread * thr = new boost::thread(boost::bind(&serviceImpl::serviceMain, this));
			thr->detach();
		}
		void onStopImpl() { run_ = false; boost::this_thread::sleep(boost::posix_time::milliseconds(5000)); }
		void onPauseImpl() { run_ = false; }
		void onContinueImpl() { run_ = true; }
		void onShutdownImpl() { run_ = false; }

		static std::string name() { return "dtccService"; };
		bool canStop() { return true; };
		bool canShutdown() { return true; };
		bool canPauseContinue() { return true; };

	private:
		bool run_ = true;
	};
}

#endif