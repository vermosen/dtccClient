#include "asio.hpp"
#include "application/logger.hpp"

namespace dtcc
{
	namespace web
	{
		namespace asio
		{
			//registerType<webReader, std::string, reader, webReader::args>
			//	reader::register_(std::string("asio"));

			const boost::regex reader::expr_("(Content-Length: )(\\d+$)(\r)");

			reader::reader(const boost::shared_ptr<protocol> & cnx, const urlReadDelegate & write) 
				: web::reader(write)
				, cnx_(cnx) {}

			reader::~reader() {}

			void reader::getAsync(const boost::shared_ptr<query> & qr)
			{
				// save the query
				query_ = qr;

				// build the query
				std::ostream request_stream(&request_);
				request_stream << "GET /";
				request_stream << query_->path();
				request_stream << " HTTP/1.1\r\n";
				request_stream << "Host: " << cnx_->host() << "\r\n";
				request_stream << "Accept: */*\r\n";

				if (cnx_->keepAlive())
				{
					request_stream << "Connection: keep-alive\r\n";

					// keep the socket alive
					cnx_->socket().set_option(boost::asio::socket_base::keep_alive(true));
				}

				request_stream << "\r\n";

				boost::asio::async_write(cnx_->socket(), request_,
					cnx_->strand().wrap(boost::bind(&reader::handle_write_request, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}

			void reader::handle_write_request(const boost::system::error_code& err, size_t bytes_transferred)
			{
				if (!err)
				{
					boost::asio::async_read_until(cnx_->socket(), response_, "\r\n",
						cnx_->strand().wrap(boost::bind(&reader::handle_read_status_line, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)));
				}
				else
				{
					write_(err, content_);
				}
			}
			void reader::handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
			{
				if (!err)
				{
					// Check that the response is OK.
					std::istream response_stream(&response_);

					std::string http_version; response_stream >> http_version;
					unsigned int status_code; response_stream >> status_code;
					std::string status_message; std::getline(response_stream, status_message);

					if (!response_stream || http_version.substr(0, 5) != "HTTP/")
					{
						write_(boost::system::errc::make_error_code(boost::system::errc::wrong_protocol_type), content_);
					}

					if (status_code == 200)
					{
						// Read the response headers, which are terminated by a blank line.
						boost::asio::async_read_until(cnx_->socket(), response_, "\r\n\r\n",
							cnx_->strand().wrap(boost::bind(&reader::handle_read_headers, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred)));
					}
					else if (status_code == 302)							// redirection
					{
						// Read the response headers, which are terminated by a blank line.
						boost::asio::async_read_until(cnx_->socket(), response_, "\r\n\r\n",
							cnx_->strand().wrap(boost::bind(&reader::handle_redirection, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred)));
					}
					else if (status_code == 403)							// forbidden
					{
						throw std::exception();
					}
					else
					{
						throw std::exception();
					}
				}
				else
				{
					write_(err, content_);
				}
			}
			void reader::handle_redirection(const boost::system::error_code& err, size_t bytes_transferred)
			{
				if (!err)
				{
					// Process the response headers.
					std::istream response_stream(&response_);

					// unload the bufstream in header_ until we reached the content (standalone empty line)
					// TODO: we may reach the end of the packet before the end of the header
					std::string h; while (std::getline(response_stream, h))
					{
						std::cout << h << std::endl;
					}
				}
				else
				{
					write_(err, content_);
				}
			}

			void reader::handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred)
			{
				if (!err)
				{
					// Process the response headers.
					std::istream response_stream(&response_);

					// unload the bufstream in header_ until we reached the content (standalone empty line)
					// TODO: we may reach the end of the packet before the end of the header
					std::string h; while (std::getline(response_stream, h) && h != "\r")
					{
						boost::smatch match;

						if (h.find("transfer-encoding: chunked") != std::string::npos) content_.chunked_ = true;
						else if (boost::regex_search(h, match, expr_))
						{
							content_.length_ = boost::lexical_cast<int>(match[2]);			// set the expected length of the response
						}

						header_ << h;
					}

					// Start reading remaining data until EOF.
					boost::asio::async_read(cnx_->socket(), response_,
						boost::asio::transfer_at_least(1),
						cnx_->strand().wrap(boost::bind(&reader::handle_read_content, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)));
				}
				else
				{
					write_(err, content_);
				}
			}
			void reader::handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
			{
				if (!err)
				{
					content_.stream_ << &response_;

					if (content_.stream_.tellg() < content_.length_)
					{
						boost::asio::async_read(cnx_->socket(), response_,
							boost::asio::transfer_at_least(1),
							cnx_->strand().wrap(boost::bind(&reader::handle_read_content, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred)));
					}
					else
					{
						write_(boost::system::errc::make_error_code(boost::system::errc::success), content_);
					}
				}
				else if (err == boost::asio::error::eof)
				{
					// connection has been closed
					content_.stream_ << &response_;
					write_(err, content_);
				}
				else if (err != boost::asio::error::eof)
				{
					LOG_ERROR() << "an error has occurred. boost error code: " << err;
				}
			}
		}
	}
}