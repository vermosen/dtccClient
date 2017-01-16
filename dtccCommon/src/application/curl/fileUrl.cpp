#include "fileUrl.hpp"

namespace dtcc
{
	fileUrl::fileUrl() : curl(), buffer_(std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary)
	{
	}

	void fileUrl::appendHeader(char * data, size_t sz)
	{
		//std::cout << header << std::endl;
	}

	void fileUrl::appendBody(char * data, size_t sz)
	{
		buffer_.write(data, sz);
	}

	std::string fileUrl::get(const std::string & url)
	{
		if (!curl_)
			throw std::exception("uninitialized connection");

		buffer_.str(""); buffer_.clear();

		std::function<void(char *, size_t)> writeBody(std::bind(&fileUrl::appendBody, this, std::placeholders::_1, std::placeholders::_2));
		std::function<void(char *, size_t)> writeHeader(std::bind(&fileUrl::appendHeader, this, std::placeholders::_1, std::placeholders::_2));

		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, fileUrl::writeMemoryCallback);
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &writeBody);

		curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, fileUrl::writeHeaderCallback);
		curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &writeHeader);

		curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

		CURLcode res = curl_easy_perform(curl_);

		std::string ret(buffer_.str());
		
		return ret;
	}
}