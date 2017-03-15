#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <string>
#include <functional>
#include <iostream>
#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <curl/curl.h>
#include "application/logger.hpp"

namespace dtcc
{
	class query
	{
	public:
		explicit query(const std::string & host, int port) : host_(host), port_(port) {}
		virtual ~query() {}
		virtual const std::string & url() = 0;

		int port() const { return port_; }
		const std::string & host() const { return host_; }
	
	protected:
		int port_;
		std::string host_;
	};

	class connection
	{
	public:
		connection() {};
		virtual ~connection() {};

		virtual boost::shared_ptr<std::string> fetch(query & q) = 0;

	protected:
		boost::shared_ptr<std::string> header_; 
		boost::shared_ptr<std::string> content_;
	};
}

#endif