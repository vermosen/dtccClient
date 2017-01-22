#ifndef ZIP_CENTRAL_DIRECTORY_FILE_HEADER_HPP_
#define ZIP_CENTRAL_DIRECTORY_FILE_HEADER_HPP_

#include <string>
#include <boost/date_time.hpp>

#include "utils/converter.hpp"
#include "application/compression/zip.hpp"
#include "application/compression/zip/method.hpp"
#include "application/compression/zip/localFileHeader.hpp"

namespace dtcc
{
	namespace zip
	{
		class centralDirectoryFileHeader
		{
		public:

			enum version : char
			{
				MSDOS = 0,
				AMIGA = 1,
				OPENVMS = 2,
				UNIX = 3,
				VMCMS = 4,
				ATARIST = 5,
				OS2HPFS = 6,
				MACHINTOSH = 7,
				ZSYSTEM = 8,
				CPM = 9,
				WINDOWSNTFS = 10,
				MVS = 11,
				VSE = 12,
				ACORNRISK = 13,
				VFAT = 14,
				ALTERNATEMVS = 15,
				BEOS = 16,
				TANDEM = 17,
				OS400 = 18,
				OSXDARWIN = 19,
				unknown
			};

			// from https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
			static constexpr std::array<char, 4> signature = { 0x50, 0x4b, 0x01, 0x02 };

			centralDirectoryFileHeader(std::string::const_iterator it)
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

			// accessors
			uint32_t offsetLocalHeader() const { return offsetLocalHeader_; }
			const std::string & name() const { return name_; }

		private:
			std::tuple<version, uint8_t> version_;
			uint16_t minimal_;
			std::bitset<16> flag_;
			zipMethod compression_;
			uint16_t modTime_;
			uint16_t modDate_;
			uint32_t crc32_;
			uint32_t compressSize_;
			uint32_t uncompressSize_;
			uint16_t diskStartNum_;
			uint16_t internalAttribute_;
			uint32_t externalAttribute_;
			uint32_t offsetLocalHeader_;
			std::string name_;
			std::string extraField_;
			std::string fileComment_;
		};
	}
}

#endif