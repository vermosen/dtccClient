#include "centralDirectoryFileHeader.hpp"

namespace dtcc
{
	namespace zip
	{
		centralDirectoryFileHeader::centralDirectoryFileHeader(std::string::const_iterator it)
		{
			version v; uint8_t n;
			int16_t fileNameLen, extraFieldLen, commentLen;

			memcpy(&v, &*it, sizeof(char) * 1);
			std::advance(it, 1); memcpy(&n, &*it, sizeof(char) * 1);
			version_ = std::make_tuple(v, n);

			std::advance(it, 1); memcpy(&minimal_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&flag_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&compression_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&modTime_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&modDate_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&crc32_, &*it, sizeof(char) * 4);
			std::advance(it, 4); memcpy(&compressSize_, &*it, sizeof(char) * 4);
			std::advance(it, 4); memcpy(&uncompressSize_, &*it, sizeof(char) * 4);
			std::advance(it, 4); memcpy(&fileNameLen, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&extraFieldLen, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&commentLen, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&diskStartNum_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&internalAttribute_, &*it, sizeof(char) * 2);
			std::advance(it, 2); memcpy(&externalAttribute_, &*it, sizeof(char) * 4);
			std::advance(it, 4); memcpy(&offsetLocalHeader_, &*it, sizeof(char) * 4);
			std::advance(it, 4);

			// copy the variable size fields
			name_.resize(fileNameLen);
			std::copy(it, it + fileNameLen, name_.begin());
			std::advance(it, fileNameLen);

			extraField_.resize(extraFieldLen);
			std::copy(it, it + extraFieldLen, extraField_.begin());
			std::advance(it, extraFieldLen);

			fileComment_.resize(commentLen);
			std::copy(it, it + commentLen, fileComment_.begin());
			std::advance(it, commentLen);
		}

		uint32_t			centralDirectoryFileHeader::offsetLocalHeader() const { return offsetLocalHeader_	; }
		const std::string & centralDirectoryFileHeader::name			 () const { return name_				; }
		uint32_t			centralDirectoryFileHeader::compressSize	 () const { return compressSize_		; }
	}
}