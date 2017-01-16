#ifndef CURL_HPP_
#define CURL_HPP_

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
	class curl
	{
	public:
		curl();
		virtual ~curl();

		virtual std::string get(const std::string & url) = 0;

	protected:
		virtual void appendBody(char *, size_t) = 0;
		virtual void appendHeader(char *, size_t) = 0;

		// callbacks
		static size_t writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer);
		static size_t writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer);

		CURL * curl_;
	};
}

#endif