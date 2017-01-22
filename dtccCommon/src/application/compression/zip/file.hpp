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

			bool operator <(const file& other) const
			{
				return central_.name() < other.central_.name();
			}

			const std::string & name() const { return central_.name(); }
			~file() {}
		private:
			centralDirectoryFileHeader central_;
			localFileHeader local_;
		};
	}
}

#endif