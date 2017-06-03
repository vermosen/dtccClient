#include "application/web/readers/curl.hpp"

namespace dtcc
{
	namespace web
	{
		//registerType<web::reader, std::string, curl, web::reader::args>
		//	curl::register_(std::string("curl"));

		//curl::curl(const web::reader::args & args)
		//	: web::reader(args), curl_(curl_easy_init())
		//{
		//	buffer_ = boost::shared_ptr<std::string>(new std::string);
		//	buffer_->reserve(1000/*boost::get<3>(args)*/);
		//}

		//curl::~curl()
		//{
		//	curl_easy_cleanup(curl_);
		//}

		//void curl::appendHeader(char * data, size_t sz)
		//{
		//	//std::cout << header << std::endl;
		//}

		//void curl::appendBody(char * data, size_t sz)
		//{
		//	buffer_->append(data, sz);
		//}

		//size_t curl::writeMemoryCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer)
		//{
		//	size_t realsize = size * nmemb;
		//	(*writer)(contents, realsize);
		//	return realsize;
		//}

		//size_t curl::writeHeaderCallback(char * contents, size_t size, size_t nmemb, std::function<void(char *, size_t)> * writer)
		//{
		//	size_t realsize = size * nmemb;
		//	(*writer)(contents, realsize);
		//	return realsize;
		//}

		//boost::shared_ptr<std::string> curl::fetch(query & q)
		//{
		//	if (!curl_)
		//		throw std::exception("uninitialized connection");

		//	std::function<void(char *, size_t)> writeBody(
		//		std::bind(&curl::appendBody,
		//			this, std::placeholders::_1,
		//			std::placeholders::_2));

		//	std::function<void(char *, size_t)> writeHeader(
		//		std::bind(&curl::appendHeader,
		//			this, std::placeholders::_1,
		//			std::placeholders::_2));

		//	curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl::writeMemoryCallback);
		//	curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &writeBody);
		//	curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, curl::writeHeaderCallback);
		//	curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &writeHeader);

		//	std::string s = q.url(protocol_, host_);
		//	curl_easy_setopt(curl_, CURLOPT_URL, q.url(protocol_, host_).c_str());

		//	CURLcode res = curl_easy_perform(curl_);

		//	boost::shared_ptr<std::string> retPtr = buffer_;

		//	// once the buffer is filled, we create a new object
		//	buffer_ = boost::shared_ptr<std::string>(new std::string);
		//	return retPtr;
		//}
	}
}