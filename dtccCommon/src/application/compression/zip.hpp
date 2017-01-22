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
		// from https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
		class zip
		{
		public:
			zip(const boost::shared_ptr<std::string> & file) 
				: file_(file) {}
			~zip() {}

			// for now, we return a single string, i.e. the archive only contains 1 file
			std::set<std::string> getFileSystem()
			{
				std::set<std::string> ret;

				if (readArchiveStructure())
				{
					for (auto it = dir_->getFileSystem().cbegin(); it != dir_->getFileSystem().cend(); it++)
					{
						ret.insert(it->name());
					}
				}
				
				return ret;
			}

			std::stringstream decompress(const std::string & file)
			{
				std::stringstream sOut;

				if (dir_)
				{
					// decompress
					boost::iostreams::zlib_params p(
						boost::iostreams::zlib::default_compression,
						boost::iostreams::zlib::deflated,
						boost::iostreams::zlib::default_window_bits,
						boost::iostreams::zlib::default_mem_level,
						boost::iostreams::zlib::default_strategy,
						true // no header
					);

					std::string sIn = dir_->getData(file);
					boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sOut);
					boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
					in.push(boost::iostreams::zlib_decompressor(p));
					in.push(boost::iostreams::array_source(sIn.c_str(), sIn.size()));
					boost::iostreams::copy(in, out);
				}
				
				return std::move(sOut);
			}

			bool readArchiveStructure()
			{
				// we start from the bottom and look for the EOCD signature
				for (auto It = file_->crbegin(); It != file_->crend(); It++)
				{
					if (*It == 0x50 && std::equal(It - 3, It, endOfCentralDirectory::eocd::signature.crbegin()))
					{
						dir_ = boost::shared_ptr<endOfCentralDirectory>(new endOfCentralDirectory(
							file_->cbegin(), file_->cend(), utils::make_forward(It)));

						return true;
					}
				}
				return false;
			}

			bool open()
			{
				// coherence check: should start with a file header
				if (std::equal(file_->cbegin(), file_->cbegin() + 4, localFileHeader::signature.begin()))
					return true;
				else
					return false;
			};

			void close()
			{

			};

		private:
			boost::shared_ptr<std::string> file_;
			boost::shared_ptr<endOfCentralDirectory> dir_;
			bool entryOpen_;
		};
	}
}

#endif