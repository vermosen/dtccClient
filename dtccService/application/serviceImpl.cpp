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
		run_ = false;

		// stop all the workers
		for (auto & i : workers_) i->stop();

		boost::this_thread::sleep(boost::posix_time::milliseconds(20000));

		// TODO: purge the data buffer
		w_.close();
	}

	void serviceImpl::startWorkers()
	{
		launchDebugger();

		try
		{
			w_.connect(settings_.database_); // TODO: timeout
		}
		catch (const std::exception & ex)
		{
			int i = 0;
		}

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
	}

	void serviceImpl::startWriter()
	{

	}
}
#endif