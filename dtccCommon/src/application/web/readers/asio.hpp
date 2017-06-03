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

#include "application/web/reader.hpp"
#include "application/web/protocol.hpp"

namespace dtcc
{
	namespace web
	{
		class asio : public reader
		{
		public:
			asio(const boost::shared_ptr<protocol> & cnx, const urlReadDelegate & write);
			~asio();

			virtual void getAsync(const boost::shared_ptr<query> & q);

		private:
			// client callbacks
			void handle_write_request	(const boost::system::error_code& err, size_t bytes_transferred);
			void handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred);
			void handle_redirection		(const boost::system::error_code& err, size_t bytes_transferred);
			void handle_read_headers	(const boost::system::error_code& err, size_t bytes_transferred);
			void handle_read_content	(const boost::system::error_code& err, size_t bytes_transferred);

		private:
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
			//static registerType<webReader, std::string, asio, webReader::args> register_;
		};
	}
}
#endif