#ifndef COMPRESSION_UNZIP_HPP_
#define COMPRESSION_UNZIP_HPP_

#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <ioapi.h>
#include <unzip.h>
#include <time.h>

# include "application/compression.hpp"


namespace dtcc
{
	// a c++ wrapper for minizip
	// need to compile zlib/contrib/vstudio/vcxx/..
	// and link the result zlibstat.lib
	class unzip : public compression
	{
	public:
		unzip();
		~unzip(void);

		bool open(const char* filename);
		void close();
		bool isOpen();

		bool openEntry(const char* filename);
		void closeEntry();
		bool isOpenEntry();
		unsigned int getEntrySize();

		const std::vector<std::string>& getFilenames();
		const std::vector<std::string>& getFolders();

		unzip & operator >> (std::ostream& os);

	private:
		void readEntries();

	private:
		unzFile			zipFile_;
		bool			entryOpen_;

		std::vector<std::string> files_;
		std::vector<std::string> folders_;
	};
}

#endif