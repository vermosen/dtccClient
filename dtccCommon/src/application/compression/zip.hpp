#ifndef COMPRESSION_ZIP_HPP_
#define COMPRESSION_ZIP_HPP_

#include <ioapi.h>
#include <zip.h>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>

# include "application/compression.hpp"

namespace dtcc
{
	class zip : compression
	{
	public:
		zip();
		~zip(void);

		bool open(const char* filename, bool append = false);
		void close();
		bool isOpen();

		bool addEntry(const char* filename);
		void closeEntry();
		bool isOpenEntry();

		zip& operator<<(std::istream& is);

	private:
		void getTime(tm_zip& tmZip);

	private:
		zipFile			zipFile_;
		bool			entryOpen_;
	};
}

#endif