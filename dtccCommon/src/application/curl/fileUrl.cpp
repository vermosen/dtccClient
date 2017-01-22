#include "fileUrl.hpp"

namespace dtcc
{
	fileUrl::fileUrl() : curl()
	{
		buffer_ = boost::shared_ptr<std::string>(new std::string);
	}

	void fileUrl::appendHeader(char * data, size_t sz)
	{
		//std::cout << header << std::endl;
	}

	void fileUrl::appendBody(char * data, size_t sz)
	{
		buffer_->append(data, sz);
	}

	boost::shared_ptr<std::string> fileUrl::fetch(const std::string & url, long size)
	{
		if (!curl_)
			throw std::exception("uninitialized connection");

		buffer_->reserve(size);

		std::function<void(char *, size_t)> writeBody(std::bind(&fileUrl::appendBody, this, std::placeholders::_1, std::placeholders::_2));
		std::function<void(char *, size_t)> writeHeader(std::bind(&fileUrl::appendHeader, this, std::placeholders::_1, std::placeholders::_2));

		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, fileUrl::writeMemoryCallback);
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &writeBody);

		curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, fileUrl::writeHeaderCallback);
		curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &writeHeader);

		curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

		CURLcode res = curl_easy_perform(curl_);

		// once the buffer is filled, we clean the buffer
		boost::shared_ptr<std::string> retPtr = buffer_;
		buffer_ = boost::shared_ptr<std::string>(new std::string);
		return retPtr;
	}
}