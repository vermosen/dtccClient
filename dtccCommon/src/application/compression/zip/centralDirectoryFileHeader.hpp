#ifndef ZIP_CENTRAL_DIRECTORY_FILE_HEADER_HPP_
#define ZIP_CENTRAL_DIRECTORY_FILE_HEADER_HPP_

#include <string>
#include <bitset>

#include <boost/date_time.hpp>

#include "application/compression/zip/method.hpp"

#include "utils/converter.hpp"

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

			// ctor
			centralDirectoryFileHeader(std::string::const_iterator it);

			// accessors
			uint32_t offsetLocalHeader() const;
			const std::string & name() const;
			uint32_t compressSize() const;

		private:
			std::tuple<version, uint8_t> version_;
			uint16_t minimal_;
			std::bitset<16> flag_;
			method compression_;
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