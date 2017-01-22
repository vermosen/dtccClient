#ifndef ZIP_END_OF_CENTRAL_DIRECTORY_HPP_
#define ZIP_END_OF_CENTRAL_DIRECTORY_HPP_

#include <bitset>

#include "utils/converter.hpp"
#include "application/compression/zip/centralDirectoryFileHeader.hpp"
#include "application/compression/zip/file.hpp"

namespace dtcc
{
	namespace zip
	{
		class endOfCentralDirectory
		{
		public:
			struct eocd
			{
				static constexpr std::array<char, 4> signature = { 0x50, 0x4b, 0x05, 0x06 };

				uint16_t nDisk_;
				uint16_t startCentralDirectoryDisk_;
				uint16_t nFilesOnThisDisk_;
				uint16_t nFiles_;
				uint32_t centralDirSize_;
				uint32_t centralDirPos_;
				std::string comment_;
			};

			struct eocd64
			{
				static constexpr std::array<char, 4> signature = { 0x50, 0x4b, 0x06, 0x06 };
				int64_t size_;
			};		

			struct eocd64Locator
			{
				static constexpr std::array<char, 4> signature = { 0x50, 0x4b, 0x06, 0x07 };
				int32_t nDisk_;
			};

			endOfCentralDirectory(std::string::const_iterator archiveStart,
								  std::string::const_iterator archiveEnd,
				std::string::const_iterator eocd)	// initially eocd.begin()
			{
				auto it = eocd;
				int16_t commentLenght;
				std::advance(it, 4); memcpy(&eocd_.nDisk_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&eocd_.startCentralDirectoryDisk_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&eocd_.nFilesOnThisDisk_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&eocd_.nFiles_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&eocd_.centralDirSize_, &*it, sizeof(char) * 4);
				std::advance(it, 4); memcpy(&eocd_.centralDirPos_, &*it, sizeof(char) * 4);
				std::advance(it, 4); memcpy(&commentLenght, &*it, sizeof(char) * 2);
				std::advance(it, 2);

				eocd_.comment_.resize(commentLenght);
				std::copy(it, it + commentLenght, eocd_.comment_.begin());
				std::advance(it, commentLenght);

				// read eocd64
				if (it != archiveEnd && std::equal(it, it + 3, eocd64::signature.cbegin()))
				{
					// TODO
					throw std::exception("unsupported archive format");
				}

				// go to the central directory position
				it = archiveStart + eocd_.centralDirPos_;

				for (it = archiveStart + eocd_.centralDirPos_; it != eocd; it++)
				{
					if (*it == 0x50 && std::equal(it, it + 3, centralDirectoryFileHeader::signature.cbegin()))
					{
						it += 4;
						files_.insert(file(archiveStart, it)); it--; // need to rollback from 1 
					}
				}
			};

			std::string getData(const std::string & fileName)
			{
				auto it = std::find_if(files_.begin(), files_.end(),
				[&fileName](const file& item)
				{
					return item.name() == fileName;
				});

				return std::move(it->getCompressedData());
			}

			~endOfCentralDirectory() {}
			const std::set<file> & getFileSystem() const { return files_; }

		private:
			eocd eocd_;
			eocd64 eocd64_;
			std::set<file> files_;
		};
	}
}

#endif