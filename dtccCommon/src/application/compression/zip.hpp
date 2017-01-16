#ifndef COMPRESSION_ZIP_HPP_
#define COMPRESSION_ZIP_HPP_

#include <sstream>
#include <iostream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

# include "application/compression.hpp"


namespace dtcc
{
	class zip : compression
	{
	public:
		zip();
		~zip(void);

		std::string expand(const std::string &);
	};
}

#endif