#ifndef ZIP_HPP_
#define ZIP_HPP_

#include <bitset>
#include <set>
#include <array>
#include <functional>
#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>

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

			bool readArchiveStructure()
			{
				// we start from the bottom and look for the EOCD signature
				for (auto It = file_->crbegin(); It != file_->crend(); It++)
				{
					if (*It == 0x50 && std::equal(It - 3, It, endOfCentralDirectory::signature.crbegin()))
					{
						dir_ = boost::shared_ptr<endOfCentralDirectory>(new endOfCentralDirectory(
							file_->cbegin(), utils::make_forward(It)));

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