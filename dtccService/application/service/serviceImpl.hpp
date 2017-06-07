#pragma once
#ifndef SERVICE_IMPL_HPP_
#define SERVICE_IMPL_HPP_

#include <boost/thread.hpp>

#include "application/service/worker.hpp"
#include "application/logger.hpp"
#include "application/service/writer.hpp"
#include "application/service.hpp"
#include "utils/debugger.hpp"

#include "settings.hpp"

namespace dtcc
{
	class serviceImpl : public service
	{
		friend service;	
	public:
		serviceImpl(const settings &settings);

	protected:
		virtual void onStart(DWORD dwArgc, LPSTR * pszArgv);
		virtual void onStop();

		virtual void onPause()	  { pause_ = false	; cv2_.notify_one(); }
		virtual void onContinue() { pause_ = true	; cv2_.notify_one(); }
		virtual void onShutdown() { run_   = false	; cv1_.notify_one(); }

	private:
		void connectWriter();
		void startWorkers();

		settings settings_;
		std::vector<boost::shared_ptr<worker> > workers_;
		writer w_;

		boost::condition_variable cv1_, cv2_;
		std::atomic<bool> run_, pause_;
		boost::mutex m1_, m2_;
	};
}

#endif