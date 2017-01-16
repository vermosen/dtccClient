#include "curl.hpp"

#include <functional>

namespace dtcc
{
	curl::curl() : curl_(curl_easy_init()) {}

	curl::~curl()
	{
		curl_easy_cleanup(curl_);
	}
	
	size_t curl::writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer)
	{
		size_t realsize = size * nmemb;
		(*writer)(contents, realsize);
		return realsize;
	}

	size_t curl::writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer)
	{
		size_t realsize = size * nmemb;
		(*writer)(contents, realsize);
		return realsize;
	}
}