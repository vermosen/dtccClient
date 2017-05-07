#pragma once
#ifndef APPLICATION_WRITER_HPP_
#define APPLICATION_WRITER_HPP_

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "application/logger.hpp"
#include "database/record/tradeRecord.hpp"

namespace dtcc
{
	typedef boost::function<void(const std::vector<database::tradeRecord> &)> writeRecordsDelegate;

	class writer
	{
	public:
		void write(const std::vector<database::tradeRecord> & s)
		{
			boost::mutex::scoped_lock lock(m_);
			LOG_INFO() << "adding " << s.size() << " new trades";
		};

	private:
		boost::mutex m_;
	};
}


#endif