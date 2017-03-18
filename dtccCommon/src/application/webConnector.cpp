#pragma once
#ifndef WEB_CONNECTOR_CPP_
#define WEB_CONNECTOR_CPP_

#include "application/webConnector.hpp"

#pragma init_seg(compiler)
template<>
dtcc::abstractFactory<dtcc::webConnector, std::string>::map_type *
	dtcc::abstractFactory<dtcc::webConnector, std::string>::map_ = new abstractFactory::map_type();

#endif
