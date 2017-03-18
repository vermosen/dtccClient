#include "registrable/base.hpp"

#include "pattern/abstractFactory.hpp"

#pragma init_seg(compiler)
template<>
boost::shared_ptr<dtcc::abstractFactory<testSuite::base, std::string >::map_type>
	dtcc::abstractFactory<testSuite::base, std::string>::map_ = 
	boost::shared_ptr<dtcc::abstractFactory<testSuite::base, std::string >::map_type>(new abstractFactory::map_type());
