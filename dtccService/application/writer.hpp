#pragma once
#ifndef APPLICATION_WRITER_HPP_
#define APPLICATION_WRITER_HPP_

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "application/logger.hpp"

namespace dtcc
{
	typedef boost::function<void(const std::string &)> writeDelegate;

	class writer
	{
	public:
		void write(const std::string & s)
		{
			boost::mutex::scoped_lock lock(m_);
			LOG_INFO() << "new message: " << s;
		};

	private:
		boost::mutex m_;
	};
}


#endif