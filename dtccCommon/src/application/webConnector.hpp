#pragma once
#ifndef WEB_CONNECTOR_HPP_
#define WEB_CONNECTOR_HPP_

#include <boost/shared_ptr.hpp>

#include "application/query.hpp"
#include "pattern/abstractFactory.hpp"

namespace dtcc
{
	class webConnector
	{
	public:
		webConnector() {};
		virtual ~webConnector() {};

		virtual boost::shared_ptr<std::string> fetch(query & q) = 0;

	protected:
		boost::shared_ptr<std::string> header_; 
		boost::shared_ptr<std::string> content_;
	};
}

#endif