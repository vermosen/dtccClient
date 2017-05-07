#ifndef ZIP_LOCAL_FILE_HEADER_HPP_
#define ZIP_LOCAL_FILE_HEADER_HPP_

#include <string>
#include <bitset>
#include <array>

#include "application/compression/zip/method.hpp"

namespace dtcc
{
	namespace zip
	{
		class localFileHeader
		{
		public:
			// from https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
			static constexpr std::array<char, 4> signature = { 0x50, 0x4b, 0x03, 0x04 };

			explicit localFileHeader(std::string::const_iterator it);

			std::string::const_iterator begin() const { return dataStart_; }

		private:
			int16_t						version_		;
			std::bitset<16>				flag_			;
			method						compression_	;
			uint16_t					modTime_		;
			uint16_t					modDate_		;
			uint32_t					crc32_			;
			uint32_t					compressSize_	;
			uint32_t					uncompressSize_	;
			std::string					name_			;
			std::string					extraField_		;
			std::string::const_iterator dataStart_		;
		};
	}
}

#endif