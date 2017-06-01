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
#include "application/web/reader.hpp"

namespace dtcc
{
	namespace web
	{
		//class curl : public reader
		//{
		//public:
		//	explicit curl(const reader::args & args);
		//	~curl();

		//	// TODO: make thread safe !
		//	virtual boost::shared_ptr<std::string> getAsync(query & q);

		//private:
		//	void appendBody(char *, size_t);
		//	void appendHeader(char *, size_t);

		//	// callbacks
		//	static size_t writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer);
		//	static size_t writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer);

		//	boost::shared_ptr<std::string> buffer_;
		//	CURL * curl_;

		//	// for factory registration
		//	static registerType<reader, std::string, curl, reader::args> register_;
		//};
	}
}

#endif