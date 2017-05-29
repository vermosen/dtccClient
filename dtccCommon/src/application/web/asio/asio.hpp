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

#include "application/webConnector.hpp"
#include "application/web/asio/protocol.hpp"

namespace dtcc
{
	typedef boost::function<void(const boost::system::error_code& err, std::string)> urlReadDelegate;

	class asio : public webConnector
	{
	public:
		asio(boost::shared_ptr<protocol> cnx, urlReadDelegate write);
		~asio();

		void getAsync();
		void setPath(const std::string & path);

	private:
		// client callbacks
		void handle_write_request	(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_redirection		(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_read_headers	(const boost::system::error_code& err, size_t bytes_transferred);
		void handle_read_content	(const boost::system::error_code& err, size_t bytes_transferred);

	private:
		urlReadDelegate write_;
		boost::shared_ptr<protocol> cnx_;

		std::string path_;
		/*std::string host_;
		int port_;*/

		std::stringstream content_;
		std::stringstream header_;

		boost::shared_ptr<boost::asio::streambuf> request_;
		boost::shared_ptr<boost::asio::streambuf> response_;

		bool ready_;
		bool success_;

		// for chucks management
		bool chunked_;
		int chunckSize_;

		// for size read
		size_t transfert_;
		static const boost::regex expr_;

		// for factory registration
		static registerType<webConnector, std::string, asio, webConnector::args> register_;
	};
}
#endif