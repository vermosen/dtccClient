#include "asio.hpp"

namespace dtcc
{
	registerType<webConnector, std::string, asio, webConnector::args>
		asio::register_(std::string("asio"));

	asio::asio(const webConnector::args & args)
		: webConnector(args)
		, context_(boost::asio::ssl::context::sslv23)
		, resolver_(service_)
		, socket_(service_, context_)
		, strand_(service_)
		, answered_(false)
	{
		// load the certificate from default
		context_.set_default_verify_paths();
		
		bool verifyHost = false; if (verifyHost)
		{
			// set verify mode
			context_.set_verify_mode(boost::asio::ssl::verify_peer);

			// set the callbacks
			context_.set_verify_callback(
				boost::bind(&asio::handle_checkCertificate, this, _1, _2));
		}
		else
		{
			context_.set_verify_mode(boost::asio::ssl::verify_none);
		}

		query_ = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
			new boost::asio::ip::tcp::resolver::query(host_, protocol_));

		this->connect();
	}

	asio::~asio() {}

	void asio::connect()
	{
		resolver_.async_resolve(*query_,
			boost::bind(&asio::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator));

		boost::thread t(boost::bind(&boost::asio::io_service::run, &service_));

		boost::mutex::scoped_lock lock(ioMutex_);

		condition_.wait(lock);
	}

	boost::shared_ptr<std::string> asio::fetch(query & q)
	{		 
		if (connected_)
		{
			std::ostream request_stream(&request_);
			request_stream << "GET /";
			request_stream << q.path();
			request_stream << " HTTP/1.1\r\n";
			request_stream << "Host: " << host_ << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Connection: close\r\n\r\n";

			LOG_INFO()
				<< "new query set for the distant host: "
				<< std::string(buffers_begin(request_.data()),
					buffers_begin(request_.data()) + request_.size());

			boost::chrono::high_resolution_clock timer;
			boost::chrono::time_point<boost::chrono::high_resolution_clock> start = timer.now();
			boost::mutex::scoped_lock lock(ioMutex_);

			boost::asio::async_write(socket_, request_, strand_.wrap(
				boost::bind(&asio::handle_write_request, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));

			service_.run();

			// should be useless...
			while (!answered_ && boost::chrono::duration_cast<boost::chrono::milliseconds>(
				timer.now() - start).count() < TIMEOUT)
			{
				condition_.wait(lock);
			}

			if (success_)
			{
				LOG_INFO() 
					<< "query retrieved in " 
					<< boost::lexical_cast<std::string>(
					boost::chrono::duration_cast<boost::chrono::milliseconds>(
						timer.now() - start).count()) 
					<< " ms";
			}
			else
			{
				LOG_ERROR() << "query failed";
				throw std::exception();
			}
		}

		return content_;
	}

	void asio::unckunck(boost::asio::streambuf & buf)
	{
		bool start = true;

		std::istream istr(&buf);
		istr >> std::noskipws;

		while (istr)
		{
			std::string h;
			if (!start)
			{
				std::getline(istr, h);
			}
			else
			{
				start = false;
			}

			std::getline(istr, h);
			std::stringstream ss;
			ss << std::hex << h.substr(0, h.size() - 1);
			ss >> chunckSize_;

			std::istream_iterator<char> beg(istr);

			//std::copy_n(beg, chunckSize_, std::back_inserter(*content_));
		}
	}

	// callbacks
	bool asio::handle_checkCertificate(bool preverified, boost::asio::ssl::verify_context& ctx)
	{
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

		LOG_INFO() << "Verifying " << std::string(subject_name);

		return preverified;
	}
	void asio::handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			LOG_INFO() << "host address resolved";

			boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
				boost::bind(&asio::handle_connect, this,
					boost::asio::placeholders::error));
		}
		else
		{
			LOG_ERROR() << "Error resolving host address: " << err.message();
			connected_ = false;
		}
	}
	void asio::handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			socket_.async_handshake(boost::asio::ssl::stream_base::client,
				boost::bind(&asio::handle_handshake, this,
					boost::asio::placeholders::error));
		}
		else
		{
			LOG_ERROR() 
				<< "Error connecting host: " 
				<< err.message();

			connected_ = false;
		}
	}
	void asio::handle_handshake(const boost::system::error_code& err)
	{
		if (!err)
		{
			LOG_INFO()
				<< "connection to host " << host_ << " successfull !";

			connected_ = true; condition_.notify_one();
		}
		else
		{
			LOG_ERROR() 
				<< "Error performing handshake: " 
				<< err.message();

			connected_ = false; condition_.notify_one();
		}
	}
	void asio::handle_write_request(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			boost::asio::async_read_until(socket_, response_, "\r\n",
				boost::bind(&asio::handle_read_status_line, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			LOG_ERROR() 
				<< "Error writing query: " 
				<< err.message();

			answered_ = true; success_ = false; condition_.notify_one();
		}
	}
	void asio::handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
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
				LOG_ERROR() 
					<< "Invalid response";

				answered_ = true; success_ = false; condition_.notify_one();
				return;
			}
			if (status_code != 200)
			{
				LOG_ERROR() 
					<< "Response returned with status code " 
					<< boost::lexical_cast<std::string>(status_code);

				answered_ = true; success_ = false; condition_.notify_one();
				return;
			}

			LOG_ERROR()
				<< "Status code: "
				<< boost::lexical_cast<std::string>(status_code);

			// Read the response headers, which are terminated by a blank line.
			boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
				boost::bind(&asio::handle_read_headers, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			LOG_ERROR() 
				<< "Error: " 
				<< err.message();

			answered_ = true; success_ = false; condition_.notify_one();
		}
	}
	void asio::handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h) && h != "\r")
			{
				// TODO: use regex for the space
				if (h.find("transfer-encoding: chunked") != std::string::npos) chunked_ = true;
				header_->append(h);
			}

			// Start reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&asio::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			LOG_ERROR() 
				<< "Error: " 
				<< err.message();

			answered_ = true; success_ = false; condition_.notify_one();
		}
	}
	void asio::handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Continue reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&asio::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

		}
		else if (err == boost::asio::error::eof)
		{
			if (chunked_)
			{
				unckunck(response_);
			}
			else
			{
				//std::copy_n(&response_, chunckSize_, std::back_inserter(*content_));
			}

			success_ = true; answered_ = true; condition_.notify_one();
		}
		else if (err != boost::asio::error::eof)
		{
			LOG_ERROR() 
				<< "Error: " 
				<< err.message();

			answered_ = true; success_ = false; condition_.notify_one();
		}
	}
}