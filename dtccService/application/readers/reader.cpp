#include "reader.hpp"
#include "application/logger.hpp"

namespace dtcc
{
	const boost::regex reader::expr_("(Content-Length: )(\\d+$)(\r)");

	reader::reader(boost::shared_ptr<protocol> cnx, urlReadDelegate write)
		: cnx_(cnx)
		, write_(write)
		, transfert_(1) {}

	reader::~reader() {}

	void reader::getAsync(const std::string & path)
	{
		// build the query
		std::ostream request_stream(&request_);
		request_stream << "GET /";
		request_stream << path;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << cnx_->host() << "\r\n";
		request_stream << "Accept: */*\r\n\r\n";

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
			write_("", false);
		}
	}
	void reader::handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Check that the response is OK.
			std::istream response_stream(&response_);
			std::string http_version;
			response_stream >> http_version;

			unsigned int status_code;
			response_stream >> status_code;

			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				write_("", false);
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
			else
			{
				write_("", false);
			}
		}
		else
		{
			write_("", false);
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
			write_("", false);
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

				if (h.find("transfer-encoding: chunked") != std::string::npos) chunked_ = true;
				else if (boost::regex_search(h, match, expr_))
				{
					transfert_ = boost::lexical_cast<int>(match[2]); //TODO: use regex
				}

				header_ << h;
			}

			// Start reading remaining data until EOF.
			boost::asio::async_read(cnx_->socket(), response_,
				boost::asio::transfer_at_least(transfert_),
				cnx_->strand().wrap(boost::bind(&reader::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			write_("", false);
		}
	}
	void reader::handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Continue reading remaining data until EOF.
			boost::asio::async_read(cnx_->socket(), response_,
				boost::asio::transfer_at_least(1),
				cnx_->strand().wrap(boost::bind(&reader::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else if (err == boost::asio::error::eof)
		{
			content_ << &response_;
			write_(content_.str(), true);
		}
		else if (err != boost::asio::error::eof)
		{
			LOG_ERROR() << "an error has occurred. boost error code: " << err;
		}
	}
}