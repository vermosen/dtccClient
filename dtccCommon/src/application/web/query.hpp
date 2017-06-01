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
	namespace web
	{
		class query
		{
		public:
			explicit query(const std::string & path) : path_(path) {}
			virtual ~query() {}
			std::string url(const protocol & cnx) const
			{
				return cnx. + "://" + host + "/" + path_;
			}
			const std::string & path() const { return path_; }

		private:
			std::string path_;
		};
	}
}

#endif#pragma once
