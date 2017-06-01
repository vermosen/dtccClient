#pragma once
#ifndef WEB_CONNECTOR_HPP_
#define WEB_CONNECTOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>

#include "pattern/abstractFactory.hpp"
#include "application/web/query.hpp"
#include "application/web/asio/protocol.hpp"

namespace dtcc
{
	namespace web
	{
		// TODO: return a string reference
		typedef boost::function<void(const boost::system::error_code& err, std::string)> urlReadDelegate;

		class reader
		{
		public:
			reader(boost::shared_ptr<protocol> cnx, urlReadDelegate write);
			virtual ~reader();

			virtual void getAsync(const query & q) = 0;

		protected:
			urlReadDelegate write_;
			boost::shared_ptr<protocol> cnx_;
		};
	}
}

#endif