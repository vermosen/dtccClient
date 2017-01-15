#ifndef CURL_HPP_
#define CURL_HPP_

#include <string>
#include <functional>
#include <iostream>
#include <fstream>

#include <curl/curl.h>
#include "application/logger.hpp"

namespace dtcc
{
	class curl
	{
	public:
		curl(const std::string & path = "");
		~curl();

		void writefile(const std::string & url, const std::string & path, bool unzip = false);

		inline void setPath(const std::string & path) { path_ = path; }
	private:
		void appendBody(const std::string & data, const std::string & file);
		void appendHeader(const std::string & data);

		// callbacks
		static size_t writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(std::string)> * writer);
		static size_t writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(std::string)> * writer);

		CURL * curl_;
		std::string path_;
		std::ofstream stream_;
	};
}

#endif