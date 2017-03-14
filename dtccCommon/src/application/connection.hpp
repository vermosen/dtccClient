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
	class connection
	{
	public:
		connection() {};
		virtual ~connection() {};

		// the reverse key define if the string is loaded 
		// in the reverse order
		virtual boost::shared_ptr<std::string> get(const std::string & url, long size) = 0;

	protected:
		boost::shared_ptr<std::string> header_; 
		boost::shared_ptr<std::string> content_;
	};
}

#endif