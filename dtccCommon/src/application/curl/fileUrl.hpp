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
		virtual std::string get(const std::string & url);

	private:

		virtual void appendBody(char *, size_t);
		virtual void appendHeader(char *, size_t);

		std::stringstream  buffer_;
	};
}

#endif