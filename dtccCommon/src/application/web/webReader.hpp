#pragma once
#ifndef WEB_CONNECTOR_HPP_
#define WEB_CONNECTOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "application/query.hpp"
#include "pattern/abstractFactory.hpp"

namespace dtcc
{
	class webReader
	{
	public:
		typedef boost::tuple<std::string, std::string, int, int> args;

		webReader(const args & t)
		: protocol_(boost::get<0>(t)), host_(boost::get<1>(t)), port_(boost::get<2>(t)) {};
		virtual ~webReader() {};

		virtual boost::shared_ptr<std::string> fetch(query & q) = 0;

	protected:
		int port_;
		std::string host_;
		std::string protocol_;

		boost::shared_ptr<std::string> header_; 
		boost::shared_ptr<std::string> content_;
	};
}

#endif