#ifndef COMPRESSION_ZIP_HPP_
#define COMPRESSION_ZIP_HPP_

#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <ioapi.h>
#include <zip.h>
#include <time.h>

namespace dtcc
{
	// a c++ wrapper for minizip
	// need to compile zlib/contrib/vstudio/vcxx/..
	// and link the result zlibstat.lib
	class zip
	{
	public:
		zip();
		~zip(void);

		bool open(const char * filename, bool append = false);
		void close();
		bool isOpen();

		bool addEntry(const char* filename);
		void closeEntry();
		bool isOpenEntry();

		zip & operator<<(std::istream& is);

	private:
		void getTime(tm_zip& tmZip);

	private:
		zipFile			zipFile_;
		bool			entryOpen_;
	};
}

#endif