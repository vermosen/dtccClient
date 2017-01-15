#include "fileUrl.hpp"

#include <functional>

namespace dtcc
{
	fileUrl::fileUrl(const std::string & path) : curl(path), path_(path), curl_(curl_easy_init())
	{

	}

	size_t fileUrl::writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(std::string)> * writer)
	{
		size_t realsize = size * nmemb;
		std::string data(contents, realsize);
		(*writer)(data);
		return realsize;
	}

	size_t fileUrl::writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(std::string)> * writer)
	{
		size_t realsize = size * nmemb;
		std::string data(contents, realsize);
		(*writer)(data);
		return realsize;
	}

	void fileUrl::appendBody(const std::string & data, const std::string & file)
	{
		stream_ << data;
	}

	void fileUrl::appendHeader(const std::string & header)
	{
		//std::cout << header << std::endl;
	}

	void fileUrl::writefile(const std::string & url, const std::string & file, bool unzip)
	{
		if (curl_)
		{
			try
			{
				stream_.open(path_ + file, std::ios::out | std::ios::app | std::ios::binary);

				std::function<void(std::string, std::string)> writeBody(std::bind(&fileUrl::appendBody, this, std::placeholders::_1, file));
				std::function<void(std::string)> writeHeader(std::bind(&fileUrl::appendHeader, this, std::placeholders::_1));

				curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, fileUrl::writeMemoryCallback);
				curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &writeBody);

				curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, fileUrl::writeHeaderCallback);
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