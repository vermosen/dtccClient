#pragma once
#ifndef CURL_HPP_
#define CURL_HPP_

#include <string>
#include <functional>
#include <sstream>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <curl/curl.h>

#include "application/logger.hpp"
#include "application/webConnector.hpp"

namespace dtcc
{
	class curl : public webConnector
	{
	public:
		explicit curl(size_t bufferSize = 1024);
		~curl();

		// TODO: make thread safe !
		virtual boost::shared_ptr<std::string> fetch(query & q);

	private:

		void appendBody(char *, size_t);
		void appendHeader(char *, size_t);
		
		// callbacks
		static size_t writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer);
		static size_t writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer);

		boost::shared_ptr<std::string> buffer_;
		CURL * curl_;

		// for factory registration
		static registerType<webConnector, std::string, curl> register_;
	};
}

#endif