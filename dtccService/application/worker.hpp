#pragma once
#ifndef WEB_WORKER_HPP_
#define WEB_WORKER_HPP_

#define FUSION_MAX_VECTOR_SIZE 45

#include <sstream>

#include <boost/asio.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/chrono.hpp>

#include "application/workerBase.hpp"
#include "application/web/protocols/https.hpp"
#include "application/web/protocols/http.hpp"
#include "application/web/readers/asio.hpp"
#include "application/web/queries/intraday.hpp"
#include "application/writer.hpp"
#include "utils/debugger.hpp"
#include "settings.hpp"

namespace dtcc
{
	class worker : public workerBase<worker>
	{
	public:
		worker(settings::worker & settings, writeRecordsDelegate write, const std::string name = "worker");
		~worker();

		void work();
		void stop();

	private:
		void connect_callback(bool result);
		void reader_callback(const boost::system::error_code& err, std::string msg);		// TODO: see if the string get copied here and how to avoid copy

		// void error_callback() // TODO
		settings::worker & settings_;

		std::unique_ptr<boost::thread> t_;
		boost::condition_variable cv_;
		std::atomic<bool> terminate_;
		boost::mutex m_;

		boost::shared_ptr<boost::asio::io_service::work> ioTask_;
		boost::shared_ptr<boost::asio::io_service> io_;

		boost::shared_ptr<web::intraday> qr_;
		boost::shared_ptr<web::protocol> cnx_;
		boost::shared_ptr<web::asio> reader_;
		writeRecordsDelegate write_;

		std::string filename_;
		int counter_;
		int nFailure_;
		boost::gregorian::date dt_;
	};
}

#endif
