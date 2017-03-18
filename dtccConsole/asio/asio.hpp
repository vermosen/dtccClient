#pragma once
#ifndef ASIO_HPP_
#define ASIO_HPP_

#include <string>
#include <sstream>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/lexical_cast.hpp>

#include <openssl/ssl.h>

#include "application/logger.hpp"
#include "application/webConnector.hpp"

#define TIMEOUT 5000

namespace dtcc
{
	class asio : public webConnector
	{
	public:

		enum { max_length = 1024 };

		explicit asio(const webConnector::args & args);
		~asio();

		virtual boost::shared_ptr<std::string> fetch(query & q);

	private:		

		void connect();
		void unckunck(boost::asio::streambuf & buf);

		// callbacks
		bool handle_checkCertificate(bool, boost::asio::ssl::verify_context&	);
		void handle_resolve			(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator);
		void handle_connect			(const boost::system::error_code&			);
		void handle_handshake		(const boost::system::error_code&			);
		void handle_write_request	(const boost::system::error_code&, size_t	);
		void handle_read_status_line(const boost::system::error_code&, size_t	);
		//void handle_redirection	(const boost::system::error_code&, size_t	);
		void handle_read_headers	(const boost::system::error_code&, size_t	);
		void handle_read_content	(const boost::system::error_code&, size_t	);

	private:
		boost::asio::io_service										service_	;
		boost::asio::ssl::context									context_	;
		boost::asio::ip::tcp::resolver								resolver_	;
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket>		socket_		;
		boost::shared_ptr<boost::asio::ip::tcp::resolver::query>	query_		;

		boost::asio::streambuf request_	;
		boost::asio::streambuf response_;

		bool success_;

		// for chucks management
		bool chunked_;
		int chunckSize_;

		// for async controls
		boost::mutex 				ioMutex_	;
		boost::condition_variable 	condition_	;
		boost::atomic<bool> 		connected_	;
		boost::atomic<bool> 		answered_	;

		// for factory registration
		static registerType <webConnector, std::string, asio, webConnector::args> register_;
	};
} /* namespace dtcc */

#endif /* ASIO_HPP_ */