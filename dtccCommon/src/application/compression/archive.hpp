#ifndef ARCHIVE_HPP_
#define ARCHIVE_HPP_

#include <string>
#include <set>
#include <utility>

namespace dtcc
{
	// the archive class
	template <typename T>
	class archive
	{
	public:
		archive(const boost::shared_ptr<std::string> & stream) 
			: isOpen_(false)
		{
			method_ = new T(stream);
		};

		~archive() { close(); delete method_; }
		bool isOpen() const { return isOpen_; }

		bool open()
		{
			if (method_->open())
			{
				isOpen_ = true;
				return true;
			}
			else return false;
		}
		void close() { method_->close(); }
		void add(const std::string & file) {}
		std::string get(const std::string &) { return std::string(""); }

		std::set<std::string> fileSystem() const { return method_->getFileSystem(); }

	private:
		T * method_;
		bool isOpen_;
	};
}

#endif