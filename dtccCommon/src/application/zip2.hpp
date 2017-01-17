#ifndef ZIP2_HPP_
#define ZIP2_HPP_

#include <bitset>
#include <set>
#include <string>
#include <array>
#include <functional>
#include <iostream>
#include <fstream>

#include <ioapi.h>
#include <unzip.h>

namespace util 
{
	// boy, I like this one
	template<typename T1, int s1, typename T2, int s2>
	union converter
	{
		static_assert(sizeof(T1) * s1 == sizeof(T2) * s2, "size is incorrect");
		T1 t1[s1];
		T2 t2[s2];
	};
}

namespace dtcc
{
	// from https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
	static constexpr std::array<char, 4> fileHeader = { '\x50', '\x4b', '\x03', '\x04' };
	static constexpr std::array<char, 4> dirHeader  = { '\x50', '\x4b', '\x01', '\x02' };

	class zip2
	{
	private:

		struct _date
		{

		};

		struct _time
		{

		};

		struct _fileRec
		{
			std::string fileName_;
			std::string extraField_;
			_date modDate_;
			_time modTime_;
			uint32_t crc32_;
			uint32_t compressedSize64_;
			uint32_t uncompressedSize64_;
		};

	public:
		zip2(std::istream && stream) : stream_(stream) {}
		~zip2() {}

		// for now, we return a single string, i.e. the archive only contains 1 file
		std::set<std::string> getFileSystem()
		{
			std::set<std::string> retval;
			std::array<char, 4> sign = {' ', ' ', ' ', ' '};

			while (true)
			{
				// position on the stream
				std::streampos pos = stream_.tellg();

				stream_.read(&sign[0], 4);

				if (std::equal(fileHeader.begin(), fileHeader.end(), sign.begin()))
				{
					_fileRec fr;

					// get file info
					char version[2]; stream_.read(&version[0], 2);
					char flag[2]; stream_.read(&flag[0], 2);
					//auto test = util::to_binary(flag[0]);
					//auto test2 = util::to_binary(flag[1]);
					char compression[2]; stream_.read(&compression[0], 2);
					char modTime[2]; stream_.read(&modTime[0], 2);
					char modDate[2]; stream_.read(&modDate[0], 2);

					util::converter<uint32_t, 1, char, 4> crc32;
					stream_.read(&crc32.t2[0], 4);
					fr.crc32_ = crc32.t2[0];

					util::converter<uint32_t, 1, char, 4> compressedSize64;
					stream_.read(&compressedSize64.t2[0], 4);
					fr.compressedSize64_ = compressedSize64.t2[0];

					util::converter<uint32_t, 1, char, 4> uncompressedSize64;
					stream_.read(&uncompressedSize64.t2[0], 4);
					fr.uncompressedSize64_ = uncompressedSize64.t2[0];

					util::converter<uint16_t, 1, char, 2> fileNameLen; 
					stream_.read(&fileNameLen.t2[0], 2);
					util::converter<uint16_t, 1, char, 2> extraFieldLen; 
					stream_.read(&extraFieldLen.t2[0], 2);

					// read the file name
					fr.fileName_ = std::string(fileNameLen.t1[0], ' ');
					stream_.read(&fr.fileName_[0], fileNameLen.t1[0]);

					// read the extra field
					fr.extraField_ = std::string(extraFieldLen.t1[0], ' ');
					stream_.read(&fr.extraField_[0], extraFieldLen.t1[0]);

					retval.insert(fr.fileName_);
				}
				else
				{
					break;
				}

				// get
			}

			// read the directory
			if (std::equal(dirHeader.begin(), dirHeader.end(), sign.begin()))
			{
				// TODO
			}

			return retval;
		}

		void open() {};
		void close()
		{

		};

	private:
		std::istream & stream_;
		bool entryOpen_;
	};
}

#endif