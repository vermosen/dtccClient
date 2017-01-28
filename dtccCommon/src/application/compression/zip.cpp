#include "zip.hpp"

namespace dtcc
{
	namespace zip
	{
		zip::zip (const boost::shared_ptr<std::string> & file) 
			: file_(file) {}
		zip::~zip() {}
		std::set<std::string> zip::getFileSystem()
		{
			std::set<std::string> ret;

			if (readArchiveStructure())
			{
				for (auto it = dir_->getFileSystem().cbegin(); it != dir_->getFileSystem().cend(); it++)
				{
					ret.insert(it->name());
				}
			}

			return ret;
		}
		std::stringstream zip::decompress(const std::string & file)
		{
			std::stringstream sOut;

			if (dir_)
			{
				// decompress
				boost::iostreams::zlib_params p(
					boost::iostreams::zlib::default_compression,
					boost::iostreams::zlib::deflated,
					boost::iostreams::zlib::default_window_bits,
					boost::iostreams::zlib::default_mem_level,
					boost::iostreams::zlib::default_strategy,
					true // no header
				);

				std::string sIn = dir_->getData(file);
				boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sOut);
				boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
				in.push(boost::iostreams::zlib_decompressor(p));
				in.push(boost::iostreams::array_source(sIn.c_str(), sIn.size()));
				boost::iostreams::copy(in, out);
			}

			return std::move(sOut);
		}
		bool zip::readArchiveStructure()
		{
			// we start from the bottom and look for the EOCD signature
			for (auto It = file_->crbegin(); It != file_->crend(); It++)
			{
				if (*It == 0x50 && std::equal(It - 3, It, endOfCentralDirectory::eocd::signature.crbegin()))
				{
					dir_ = boost::shared_ptr<endOfCentralDirectory>(new endOfCentralDirectory(
						file_->cbegin(), file_->cend(), utils::make_forward(It)));

					return true;
				}
			}
			return false;
		}
		bool zip::open()
		{
			// coherence check: should start with a file header
			if (std::equal(file_->cbegin(), file_->cbegin() + 4, localFileHeader::signature.begin()))
				return true;
			else
				return false;
		};
		void zip::close()
		{

		};
	}
}