#pragma once
#ifndef APP_RUNNER_HPP_
#define APP_RUNNER_HPP_

#define FUSION_MAX_VECTOR_SIZE 45

#include <fstream>
#include <streambuf>
#include <exception>
#include <array>
#include <vector>
#include <functional>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/asio.hpp>

#include "application/compression/archive.hpp"
#include "application/compression/zip.hpp"
#include "application/web/readers/asio.hpp"

#include "database/sql/recordsets/tradeRecordset.hpp"
#include "database/sql/connector.hpp"
#include "database/record/parser/parseRecords.hpp"
#include "application/logger.hpp"
#include "application/startup.hpp"
#include "application/web/queries/eod.hpp"
#include "application/web/protocols/https.hpp"

#include "settings/parser/parseSettings.hpp"
#include "settings.hpp"

namespace dtcc
{
	class appRunner
	{
	public:
		appRunner(const dtcc::settings & settings);
		~appRunner();

		void run();

		void connect_callback	(const boost::system::error_code& err);
		void load_callback		(const boost::system::error_code& err, const dtcc::web::content & ct);

	private:
		dtcc::settings settings_;

		// TODO: encapsulate the asio components into some asio core class
		boost::shared_ptr<boost::asio::io_service> io_;
		boost::shared_ptr<boost::asio::io_service::work> ioTask_;

		boost::shared_ptr<dtcc::database::sql::connector> db_;
		boost::shared_ptr<dtcc::web::asio::protocol> cnx_;
		boost::shared_ptr<dtcc::web::reader> rd_;

		boost::chrono::high_resolution_clock::time_point start_;		// chrono

		std::unique_ptr<boost::thread> t_;
		boost::condition_variable cv_;
		std::atomic<bool> finished_;
		boost::mutex m_;

		// the iterator
		std::vector<asset::description>::const_iterator it_;
		boost::gregorian::date dt_;
	};
}

#endif