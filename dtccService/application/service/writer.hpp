#pragma once
#ifndef APPLICATION_WRITER_HPP_
#define APPLICATION_WRITER_HPP_

#include <stack>

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "application/logger.hpp"
#include "database/record/tradeRecord.hpp"
#include "database/recordsets/tradeRecordset.hpp"
#include "database/connector.hpp"
#include "settings.hpp"

namespace dtcc
{
	typedef boost::function<void(const std::vector<database::tradeRecord> &)> writeRecordsDelegate;

	class writer
	{
	public:
		writer(size_t bufferSize) : sz_(bufferSize)
		{
			ctr_ = dtcc::abstractFactoryNoArgs<database::connector, std::string>::createInstance("sql server");
		}

		writer() = delete;

		~writer() 
		{
			// TODO: close the connection ?
		}

		void connect(const settings::connectionString & db) { ctr_->connect(db.database_); }
		void close() { save(); ctr_->close(); }

		void write(const std::vector<database::tradeRecord> & s)
		{
			boost::mutex::scoped_lock lock(m_);
			LOG_INFO() << "adding " << s.size() << " new trades";
			 
			for (auto & i : s)
			{
				buffer_.push_back(i);

				if (buffer_.size() > sz_) save();
			}
		};

	private:

		void save()
		{
			// unloads the buffer
			database::tradeRecordset rs(ctr_);
			auto res = rs.insert(buffer_);
			buffer_.clear();
		}

		size_t sz_;
		boost::mutex m_;
		std::vector<database::tradeRecord> buffer_;			// not the best, change to queue ?
		boost::shared_ptr<database::connector> ctr_;
	};
}


#endif