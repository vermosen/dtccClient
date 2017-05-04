#pragma once
#ifndef READER_HPP_
#define READER_HPP_

#include <memory>
#include <array>
#include <utility>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "application/connection.hpp"

namespace dtcc
{
	typedef boost::function<void(const std::string &, bool)> urlReadDelegate;

	class reader
	{
	public:
		reader(boost::shared_ptr<connection> cnx, urlReadDelegate write);
		~reader();

		void getAsync(const std::string & path);

	private:
		// client callbacks
		void handle_write_request	(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_redirection		(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_read_headers	(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_read_content	(const boost::system::error_code& err, size_t bytes_transferred);

	private:
		urlReadDelegate write_;
		boost::shared_ptr<connection> cnx_;

		std::string host_;
		int port_;

		std::stringstream content_;
		std::stringstream header_;

		boost::asio::streambuf request_;
		boost::asio::streambuf response_;

		bool ready_;
		bool success_;

		// for chucks management
		bool chunked_;
		int chunckSize_;

		// for size read
		size_t transfert_;
		static const boost::regex expr_;
	};
}
#endif