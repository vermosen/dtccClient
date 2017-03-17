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
		explicit query(const std::string & host, int port) : host_(host), port_(port) {}
		virtual ~query() {}
		virtual std::string path() const = 0;
		std::string url() const { return std::string(host() + path()); }
		int port() const { return port_; }
		const std::string & host() const { return host_; }

	protected:
		int port_;
		std::string host_;
	};
}

#endif#pragma once
