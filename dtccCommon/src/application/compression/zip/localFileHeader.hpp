#ifndef ZIP_LOCAL_FILE_HEADER_HPP_
#define ZIP_LOCAL_FILE_HEADER_HPP_

#include <string>
#include <bitset>
#include <boost/date_time.hpp>

#include "application/compression/zip.hpp"
#include "application/compression/zip/method.hpp"
#include "utils/converter.hpp"

namespace dtcc
{
	namespace zip
	{
		class localFileHeader
		{
		public:
			// from https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
			static constexpr std::array<char, 4> signature = { 0x50, 0x4b, 0x03, 0x04 };

			localFileHeader(std::string::const_iterator it)
			{
				uint16_t fileNameLen, extraFieldLen;
				std::advance(it, 4); memcpy(&version_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&flag_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&compression_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&modTime_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&modDate_, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&crc32_, &*it, sizeof(char) * 4);
				std::advance(it, 4); memcpy(&compressSize_, &*it, sizeof(char) * 4);
				std::advance(it, 4); memcpy(&uncompressSize_, &*it, sizeof(char) * 4);
				std::advance(it, 4); memcpy(&fileNameLen, &*it, sizeof(char) * 2);
				std::advance(it, 2); memcpy(&extraFieldLen, &*it, sizeof(char) * 2);
				std::advance(it, 2);

				// copy the variable size fields
				name_.resize(fileNameLen);
				std::copy(it, it + fileNameLen, name_.begin());
				std::advance(it, fileNameLen);

				extraField_.resize(extraFieldLen);
				std::copy(it, it + extraFieldLen, extraField_.begin());
				std::advance(it, extraFieldLen);
				dataStart_ = it;
			}
			std::string::const_iterator begin() const { return dataStart_; }
			//std::string::const_iterator end() const { return dataStart_ + compressSize_; }

		private:
			int16_t version_;
			std::bitset<16> flag_;
			zipMethod compression_;
			uint16_t modTime_;
			uint16_t modDate_;
			uint32_t crc32_;
			uint32_t compressSize_;
			uint32_t uncompressSize_;
			std::string name_;
			std::string extraField_;
			std::string::const_iterator dataStart_;
		};
	}
}

#endif