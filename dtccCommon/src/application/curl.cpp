#include "curl.hpp"

#include <functional>

namespace dtcc
{
	curl::curl(const std::string & path) : path_(path), curl_(curl_easy_init())
	{
		//curl_ = curl_easy_init();
	}

	curl::~curl()
	{
		curl_easy_cleanup(curl_);
	}
	
	size_t curl::writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(std::string)> * writer)
	{
		size_t realsize = size * nmemb;
		std::string data(contents, realsize);
		(*writer)(data);
		return realsize;
	}

	size_t curl::writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(std::string)> * writer)
	{
		size_t realsize = size * nmemb;
		std::string data(contents, realsize);
		(*writer)(data);
		return realsize;
	}

	void curl::appendBody(const std::string & data, const std::string & file) 
	{	
		stream_ << data;
	}

	void curl::appendHeader(const std::string & header) 
	{
		//std::cout << header << std::endl;
	}

	void curl::writefile(const std::string & url, const std::string & file, bool unzip)
	{
		if (curl_)
		{
			try
			{
				stream_.open(path_ + file, std::ios::out | std::ios::app | std::ios::binary);

				std::function<void(std::string, std::string)> writeBody(std::bind(&curl::appendBody, this, std::placeholders::_1, file));
				std::function<void(std::string)> writeHeader(std::bind(&curl::appendHeader, this, std::placeholders::_1));

				curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl::writeMemoryCallback);
				curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &writeBody);

				curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, curl::writeHeaderCallback);
				curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &writeHeader);

				curl_easy_setopt(curl_, CURLOPT_URL, (url + file).c_str());

				CURLcode res = curl_easy_perform(curl_);

				stream_.close();
			}
			catch (const std::exception & ex)
			{
				throw ex;
			}
		}
	}
}