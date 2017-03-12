#ifndef ZIP_LOCAL_FILE_HEADER_CPP_
#define ZIP_LOCAL_FILE_HEADER_CPP_

#include "application/compression/zip/localFileHeader.hpp"

#include "application/compression/zip.hpp"
#include "utils/converter.hpp"

namespace dtcc
{
	namespace zip
	{
		localFileHeader::localFileHeader(std::string::const_iterator it)
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
	}
}

#endif