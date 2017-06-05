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
#include "application/web/protocol.hpp"

namespace dtcc
{
	namespace web
	{
		class query
		{
		public:
			explicit query() {}
			virtual ~query() {}
			std::string url(const protocol & cnx) const
			{
				return cnx.name() + "://" + cnx.host() + "/" + path();
			}
			virtual std::string path() const = 0;
		};
	}
}

#endif#pragma once
