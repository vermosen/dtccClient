#include "zip.hpp"

namespace dtcc
{
	// Default constructor
	zip::zip() : compression()
	{
	}

	// Default destructor
	zip::~zip(void)
	{
	}

	std::string zip::expand(const std::string & str)
	{
		std::stringstream ss(str);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
		in.push(boost::iostreams::zlib_decompressor());
		in.push(ss);
		boost::iostreams::copy(in, std::cout);
		
		return "";
	}
};
