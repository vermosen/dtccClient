#pragma once
#ifndef WEB_CONNECTOR_CPP_
#define WEB_CONNECTOR_CPP_

#include "application/web/reader.hpp"

//#pragma init_seg(compiler)
//template<>
//dtcc::abstractFactory<dtcc::web::reader, std::string, dtcc::web::reader::args>::map_type *
//	dtcc::abstractFactory<dtcc::web::reader, std::string, dtcc::web::reader::args>::map_ = new abstractFactory::map_type();

namespace dtcc
{
	namespace web
	{
		reader::reader(boost::shared_ptr<protocol> cnx, urlReadDelegate write)
				: cnx_(cnx)
				, write_(write) {};

		reader::~reader() {};
	}
}

#endif
