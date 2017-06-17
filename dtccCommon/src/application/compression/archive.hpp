#ifndef ARCHIVE_HPP_
#define ARCHIVE_HPP_

#include <string>
#include <set>
#include <utility>
#include <sstream>

namespace dtcc
{
	// the archive class
	template <typename T>
	class archive
	{
	public:
		archive(std::string && raw) 
			: isOpen_(false)
			, method_(std::forward<std::string>(raw)) {};

		~archive() { close(); }
		bool isOpen() const { return isOpen_; }

		bool open()
		{
			if (method_.open())
			{
				isOpen_ = true;
				return true;
			}
			else return false;
		}
		void close() { method_.close(); }
		void add(const std::string & file) {}
		std::string get(const std::string & file) { return method_.decompress(file); }

		std::set<std::string> fileSystem() { return method_.getFileSystem(); }

	private:
		T method_;
		bool isOpen_;
	};
}

#endif