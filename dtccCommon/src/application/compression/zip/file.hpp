#ifndef ZIP_FILE_HPP_
#define ZIP_FILE_HPP_

#include <string>

namespace dtcc
{
	namespace zip
	{
		class file
		{
		public:
			file(std::string::const_iterator archiveStart,
				std::string::const_iterator eocdEntry) 
				: local_(archiveStart + central_.offsetLocalHeader())
				, central_(eocdEntry) {};

			~file() {}

			bool operator <(const file& other) const
			{
				return central_.name() < other.central_.name();
			}

			const std::string & name() const { return central_.name(); }
			
			std::string getCompressedData(bool header = true) const
			{
				uint32_t size = central_.compressSize();
				std::string::const_iterator start = local_.begin();
				std::string::const_iterator end = start + size;
				std::string ret; ret.resize(size);
				std::copy(start, end, ret.begin());
				return std::move(ret);
			}

		private:
			centralDirectoryFileHeader central_;
			localFileHeader local_;
		};
	}
}

#endif