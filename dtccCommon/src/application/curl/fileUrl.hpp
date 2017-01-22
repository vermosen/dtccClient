#ifndef CURL_FILEURL_HPP_
#define CURL_FILEURL_HPP_

#include <string>
#include <functional>
#include <iostream>
#include <sstream>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <curl/curl.h>

#include "application/logger.hpp"
#include "application/curl.hpp"

namespace dtcc
{
	class fileUrl : public curl
	{
	public:
		fileUrl();
		~fileUrl() {};

		// TODO: make thread safe !
		virtual boost::shared_ptr<std::string> fetch(const std::string & url, long size = 1024);

	private:

		virtual void appendBody(char *, size_t);
		virtual void appendHeader(char *, size_t);

		boost::shared_ptr<std::string> buffer_;
	};
}

#endif