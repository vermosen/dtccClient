#include "zip.hpp"

namespace dtcc
{
	const unsigned int BUFSIZE = 2048;

	// Default constructor
	zip::zip() : compression(),
		zipFile_(0),
		entryOpen_(false)
	{
	}

	// Default destructor
	zip::~zip()
	{
		close();
	}

	// Create a new zip file.
	// param:
	//		filename	path and the filename of the zip file to open
	//		append		set true to append the zip file
	// return:
	//		true if open, false otherwise
	bool zip::open(const char* filename, bool append)
	{
		close();
		zipFile_ = zipOpen64(filename, append ? APPEND_STATUS_ADDINZIP : 0);

		return isOpen();
	}

	// Close the zip file
	void zip::close()
	{
		if (zipFile_)
		{
			closeEntry();
			zipClose(zipFile_, 0);
			zipFile_ = 0;
		}
	}

	// Check if a zipfile is open.
	// return:
	//		true if open, false otherwise
	bool zip::isOpen()
	{
		return zipFile_ != 0;
	}

	// Create a zip entry; either file or folder. Folder has to 
	// end with a slash or backslash.
	// return:
	//		true if open, false otherwise
	bool zip::addEntry(const char* filename)
	{
		if (isOpen())
		{
			closeEntry();

			while (filename[0] == '\\' || filename[0] == '/')
			{
				filename++;
			}

			//?? we dont need the stinking time
			zip_fileinfo zi = { 0 };
			getTime(zi.tmz_date);

			int err = zipOpenNewFileInZip(zipFile_, filename, &zi,
				NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

			entryOpen_ = (err == ZIP_OK);
		}
		return entryOpen_;
	}

	// Close the currently open zip entry.
	void zip::closeEntry()
	{
		if (entryOpen_)
		{
			zipCloseFileInZip(zipFile_);
			entryOpen_ = false;
		}
	}

	// Check if there is a currently open file zip entry.
	// return:
	//		true if open, false otherwise
	bool zip::isOpenEntry()
	{
		return entryOpen_;
	}

	// Stream operator for dumping data from an input stream to the 
	// currently open zip entry.
	zip& zip::operator<<(std::istream& is)
	{
		int err = ZIP_OK;
		char buf[BUFSIZE];
		unsigned long nRead = 0;

		if (isOpenEntry())
		{
			while (err == ZIP_OK && is.good())
			{
				is.read(buf, BUFSIZE);
				unsigned int nRead = (unsigned int)is.gcount();

				if (nRead)
				{
					err = zipWriteInFileInZip(zipFile_, buf, nRead);
				}
				else
				{
					break;
				}
			}
		}
		return *this;
	}

	// Fill the zip time structure
	// param:
	//		tmZip	time structure to be filled
	void zip::getTime(tm_zip& tmZip)
	{
		time_t rawtime;
		time(&rawtime);
		auto timeinfo = localtime(&rawtime);
		tmZip.tm_sec = timeinfo->tm_sec;
		tmZip.tm_min = timeinfo->tm_min;
		tmZip.tm_hour = timeinfo->tm_hour;
		tmZip.tm_mday = timeinfo->tm_mday;
		tmZip.tm_mon = timeinfo->tm_mon;
		tmZip.tm_year = timeinfo->tm_year;
	}
};
