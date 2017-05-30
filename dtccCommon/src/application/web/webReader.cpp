#pragma once
#ifndef WEB_CONNECTOR_CPP_
#define WEB_CONNECTOR_CPP_

#include "application/web/webReader.hpp"

#pragma init_seg(compiler)
template<>
dtcc::abstractFactory<dtcc::webReader, std::string, dtcc::webReader::args>::map_type *
	dtcc::abstractFactory<dtcc::webReader, std::string, dtcc::webReader::args>::map_ = new abstractFactory::map_type();

#endif
