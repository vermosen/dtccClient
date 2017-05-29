#include "asio.hpp"
#include "application/logger.hpp"

namespace dtcc
{
	registerType<webConnector, std::string, asio, webConnector::args>
		asio::register_(std::string("asio"));

	const boost::regex asio::expr_("(Content-Length: )(\\d+$)(\r)");

	asio::asio(boost::shared_ptr<protocol> cnx, urlReadDelegate write)
		: webConnector(boost::tuple<std::string, std::string, int, int>())
		, cnx_(cnx)
		, write_(write)
		, transfert_(1) {}

	asio::~asio() {}

	void asio::setPath(const std::string & path)
	{
		path_ = path;
	}

	void asio::getAsync()
	{
		{
			// first clean the request and response buffers
			request_ = boost::shared_ptr<boost::asio::streambuf>(new boost::asio::streambuf());
			response_ = boost::shared_ptr<boost::asio::streambuf>(new boost::asio::streambuf());
		}

		// build the query
		std::ostream request_stream(&*request_);
		request_stream << "GET /";
		request_stream << path_;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << cnx_->host() << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: keep-alive\r\n";
		request_stream << "\r\n";

		boost::asio::async_write(cnx_->socket(), *request_,
			cnx_->strand().wrap(boost::bind(&asio::handle_write_request, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
	}

	void asio::handle_write_request(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			boost::asio::async_read_until(cnx_->socket(), *response_, "\r\n",
				cnx_->strand().wrap(boost::bind(&asio::handle_read_status_line, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			write_(err, "");
		}
	}
	void asio::handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Check that the response is OK.
			std::istream response_stream(&*response_);
			std::string http_version;
			response_stream >> http_version;

			unsigned int status_code;
			response_stream >> status_code;

			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				write_(boost::system::errc::make_error_code(boost::system::errc::wrong_protocol_type), false);
			}

			if (status_code == 200)
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_->socket(), *response_, "\r\n\r\n",
					cnx_->strand().wrap(boost::bind(&asio::handle_read_headers, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else if (status_code == 302)							// redirection
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_->socket(), *response_, "\r\n\r\n",
					cnx_->strand().wrap(boost::bind(&asio::handle_redirection, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else
			{
				throw std::exception();
			}
		}
		else
		{
			write_(err, "");
		}
	}
	void asio::handle_redirection(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&*response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h))
			{
				std::cout << h << std::endl;
			}
		}
		else
		{
			write_(err, "");
		}
	}

	void asio::handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&*response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h) && h != "\r")
			{
				boost::smatch match;

				if (h.find("transfer-encoding: chunked") != std::string::npos) chunked_ = true;
				else if (boost::regex_search(h, match, expr_))
				{
					transfert_ = boost::lexical_cast<int>(match[2]); //TODO: use regex
				}

				header_ << h;
			}

			// Start reading remaining data until EOF.
			boost::asio::async_read(cnx_->socket(), *response_,
				boost::asio::transfer_at_least(transfert_),
				cnx_->strand().wrap(boost::bind(&asio::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			write_(err, "");
		}
	}
	void asio::handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// there should be no second callback 
			boost::asio::async_read(cnx_->socket(), *response_,
				boost::asio::transfer_at_least(1),
				cnx_->strand().wrap(boost::bind(&asio::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));

			content_ << &*response_;
			write_(boost::system::errc::make_error_code(boost::system::errc::success), content_.str());
		}
		else if (err == boost::asio::error::eof)
		{
			// connection has been closed
			content_ << &*response_;
			write_(err, content_.str());
		}
		else if (err != boost::asio::error::eof)
		{
			LOG_ERROR() << "an error has occurred. boost error code: " << err;
		}
	}
}