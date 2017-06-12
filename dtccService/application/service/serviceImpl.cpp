#ifndef SERVICE_IMPL_CPP_
#define SERVICE_IMPL_CPP_

#include "serviceImpl.hpp"

namespace dtcc
{
	serviceImpl::serviceImpl(const settings &settings) 
		: service("dtccService", true, false, false)
		, w_(settings.memory_)
	{
		settings_ = settings;
	}

	void serviceImpl::onStart(DWORD dwArgc, LPSTR * pszArgv)
	{
		run_ = true;
		LOG_INFO() << "activating service... ";

		boost::thread * thr = new boost::thread(boost::bind(&serviceImpl::startWorkers, this));
		LOG_INFO() << "activation completed";
		thr->detach();
	}

	void serviceImpl::onStop()
	{
		LOG_INFO() << "called onStop method... ";

		run_ = false;

		// stop all the workers
		for (auto & i : workers_) i->stop();

		// TODO: pass a setting
		boost::this_thread::sleep(boost::posix_time::milliseconds(30000));

		// TODO: purge the data buffer
		w_.close();

		LOG_INFO() << "onStop method successfully completed";
	}

	void serviceImpl::startWorkers()
	{
		connectWriter();
		LOG_INFO() << "reached database... ";

		#ifdef _DEBUG
		launchDebugger();
		#endif

		writeRecordsDelegate f(boost::bind(&writer::write, &w_, _1));

		LOG_INFO() << "creating " << settings_.workers_.size() << "workers";

		workers_.resize(settings_.workers_.size());

		for (int i = 0; i < settings_.workers_.size(); i++)
		{
			std::string id = boost::lexical_cast<std::string>(i);
			workers_[i] = boost::shared_ptr<worker>(new worker(settings_.workers_[i], f, "worker " + id));
		}

		LOG_INFO() << "starting workers...";

		for (auto & i : workers_) i->start();

		// barrier
		boost::unique_lock<boost::mutex> lk(m1_);
		while (run_) cv1_.wait(lk);

		// tell all the worker to stop
		for (auto & it : workers_)
		{
			it->stop();
		}

		// wait another 15 secs for the cleanup
		boost::this_thread::sleep(boost::posix_time::milliseconds(20000));

		// commit all the pending records
		w_.close();
	}

	void serviceImpl::connectWriter()
	{
		try
		{
			w_.connect(settings_.database_); // TODO: timeout
		}
		catch (const std::exception & ex)
		{
			// TODO: retry
			LOG_ERROR() << ex.what();
			throw ex;
		}
	}
}
#endif