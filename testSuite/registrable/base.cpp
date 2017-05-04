#include "registrable/base.hpp"

#include "pattern/abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
dtcc::abstractFactory<testSuite::base, std::string, char>::map_type * 
	dtcc::abstractFactory<testSuite::base, std::string, char>::map_ = new abstractFactory::map_type();
