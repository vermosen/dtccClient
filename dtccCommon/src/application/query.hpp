#ifndef QUERY_HPP_
#define QUERY_HPP_

#include <string>
#include <functional>
#include <iostream>
#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <curl/curl.h>
#include "application/logger.hpp"

namespace dtcc
{
	class query
	{
	public:
		explicit query() {}
		virtual ~query() {}
		virtual std::string path() const = 0;
		std::string url(const std::string & protocol, const std::string & host) const
		{
			return protocol + "://" + host + "/" + this->path();
		}
	};
}

#endif#pragma once
