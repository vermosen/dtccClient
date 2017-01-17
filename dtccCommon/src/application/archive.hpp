#ifndef ARCHIVE_HPP_
#define ARCHIVE_HPP_

#include <string>
#include <vector>
#include <set>
#include <utility>

#include "application/zip2.hpp"

namespace dtcc
{
	// the archive class
	class archive
	{
	public:

		archive(std::istream && stream) 
			: isOpen_(false)
		{
			method_ = new zip2(std::move(stream));
		};

		~archive() { close(); delete method_; };
		bool isOpen() const { return isOpen_; }

		bool open()
		{
			isOpen_ = true;
			fileSystem_ = method_->getFileSystem();
			return true;
		}

		const std::set<std::string> & fileSystem() const { return fileSystem_; }

	private:
		void close() { method_->close(); }

		zip2 * method_;
		bool isOpen_;
		std::set<std::string> fileSystem_;
	};
}

#endif