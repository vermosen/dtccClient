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
		namespace asio
		{
			class reader : public web::reader
			{
			public:
				reader(const boost::shared_ptr<protocol> & cnx, const urlReadDelegate & write);
				~reader();

				virtual void getAsync(const boost::shared_ptr<query> &);

			private:
				// client callbacks
				void handle_write_request(const boost::system::error_code& err, size_t bytes_transferred);
				void handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred);
				void handle_redirection(const boost::system::error_code& err, size_t bytes_transferred);
				void handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred);
				void handle_read_content(const boost::system::error_code& err, size_t bytes_transferred);

			private:
				content content_;
				std::stringstream header_;

				// r/w buffers
				boost::asio::streambuf request_;
				boost::asio::streambuf response_;

				/*bool ready_; bool success_;*/

				// for header read
				static const boost::regex expr_;

				// for factory registration
				//static registerType<webReader, std::string, asio, webReader::args> register_;
			};
		}
	}
}
#endif