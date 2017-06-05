#pragma once
#ifndef SERVICE_IMPL_HPP_
#define SERVICE_IMPL_HPP_

#include <boost/thread.hpp>

#include "application/worker.hpp"
#include "application/logger.hpp"
#include "application/writer.hpp"
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

		virtual void onPause() { run_ = false; }
		virtual void onContinue() { run_ = true; }
		virtual void onShutdown() { run_ = false; }

	private:
		void startWriter();
		void startWorkers();

		bool run_;
		settings settings_;
		std::vector<boost::shared_ptr<worker>> workers_;
		writer w_;
	};
}

#endif