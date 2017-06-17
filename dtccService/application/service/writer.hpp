#pragma once
#ifndef APPLICATION_WRITER_HPP_
#define APPLICATION_WRITER_HPP_

#include <stack>

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "application/logger.hpp"
#include "database/record/tradeRecord.hpp"
#include "database/sql/recordsets/tradeRecordset.hpp"
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
			cnx_.reset(new dtcc::database::sql::connector());
		}

		writer() = delete;

		~writer() 
		{
			// TODO: close the connection ?
		}

		void connect(const settings::connectionString & db) 
		{
			cnx_->setConnectionString(db.database_);
			cnx_->connect();
		}
		void close() { save(); cnx_->close(); }

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

		void cleanup(const boost::gregorian::date & dt)
		{
			database::sql::tradeRecordset rs(cnx_);

			std::ostringstream os;
			auto * temp(new boost::gregorian::date_facet("%Y-%m-%d"));
			os.imbue(std::locale(os.getloc(), temp));
			os << "FILE_DATE = '" << dt << "'";

			rs.remove(os.str());
		}

	private:

		void save()
		{
			// unloads the buffer
			database::sql::tradeRecordset rs(cnx_);
			auto res = rs.insert(buffer_);
			buffer_.clear();
		}

		size_t sz_;
		boost::mutex m_;
		std::vector<database::tradeRecord> buffer_;			// not the best, change to queue ?
		boost::shared_ptr<database::sql::connector> cnx_;
	};
}

#endif