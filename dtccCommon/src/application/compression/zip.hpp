#ifndef ZIP_HPP_
#define ZIP_HPP_

#include <bitset>
#include <set>
#include <array>
#include <functional>
#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include "application/compression/zip/endOfCentralDirectory.hpp"
#include "application/compression/zip/file.hpp"

namespace dtcc
{
	namespace utils
	{
		template <class reverseIterator>
		typename reverseIterator::iterator_type make_forward(reverseIterator rit)
		{
			return --(rit.base());
		}
	}

	namespace zip
	{
		// zip class 
		// cf reference document https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
		class zip
		{
		public:
			explicit zip(std::string &&);
			~zip();

			// for now, we return a single string, i.e. the archive only contains 1 file
			std::set<std::string> getFileSystem();
			std::stringstream decompress(const std::string &);
			bool readArchiveStructure();
			bool open();
			void close();

		private:
			std::string file_;
			boost::shared_ptr<endOfCentralDirectory> dir_;
			bool entryOpen_;
		};
	}
}

#endif