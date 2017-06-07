#pragma once
#ifndef WORKER_IMPL_HPP_
#define WORKER_IMPL_HPP_

#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "utils/thread.hpp"

namespace dtcc
{
	template <typename W>
	class workerBase
	{
	protected:
		workerBase(const std::string & name);
		virtual ~workerBase();

	public:
		void start();
		void join();

	private:
		std::unique_ptr<boost::thread> t_;
		std::string name_;
	};

	template <typename W>
	workerBase<W>::workerBase(const std::string & name)
		: name_(name) {}

	template <typename W>
	workerBase<W>::~workerBase() { /* interrupt ?*/ }

	template <typename W>
	void workerBase<W>::start()
	{
		if (!t_)
		{
			t_ = std::unique_ptr<boost::thread>(new boost::thread(
				[&]() { static_cast<W*>(this)->work(); }));
			setThreadName(t_->get_id(), name_);
		}
	}

	template <typename W>
	void workerBase<W>::join()
	{
		if (t_) t_->join();
	}
}

#endif