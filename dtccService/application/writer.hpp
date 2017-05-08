#pragma once
#ifndef APPLICATION_WRITER_HPP_
#define APPLICATION_WRITER_HPP_

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "application/logger.hpp"
#include "database/record/tradeRecord.hpp"
#include "database/connector.hpp"
#include "settings.hpp"

namespace dtcc
{
	typedef boost::function<void(const std::vector<database::tradeRecord> &)> writeRecordsDelegate;

	class writer
	{
	public:
		writer(const settings::database & db, size_t bufferSize)
		{
			buffer_.resize(bufferSize);
			ctr_ = dtcc::abstractFactoryNoArgs<database::connector, std::string>::createInstance("sql server");
			ctr_->connect(db.database_);
		}

		writer() = delete;

		~writer() 
		{
			// TODO: close the connection ?
		}

		void write(const std::vector<database::tradeRecord> & s)
		{
			boost::mutex::scoped_lock lock(m_);
			LOG_INFO() << "adding " << s.size() << " new trades";
		};

	private:
		boost::mutex m_;
		std::vector<database::tradeRecord> buffer_;
		boost::shared_ptr<database::connector> ctr_;
	};
}


#endif