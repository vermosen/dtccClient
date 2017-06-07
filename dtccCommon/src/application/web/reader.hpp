#pragma once
#ifndef WEB_CONNECTOR_HPP_
#define WEB_CONNECTOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>

#include "pattern/abstractFactory.hpp"
#include "application/web/query.hpp"
#include "application/web/protocol.hpp"

namespace dtcc
{
	namespace web
	{
		// the response content
		class content
		{
		public:
			size_t length_;
			std::stringstream stream_;
			bool chunked_;
			size_t chunkSize_;

			bool isEmpty() const { return length_ == 0; }
		};

		// a callback function TODO: use move semantic
		typedef boost::function<void(const boost::system::error_code&, const content &)> urlReadDelegate;

		class reader
		{
		public:
			reader(const urlReadDelegate &);
			virtual ~reader();

			virtual void getAsync(const boost::shared_ptr<query> & query) = 0;

			boost::shared_ptr<query> getQuery();

		protected:
			urlReadDelegate write_;
			boost::shared_ptr<query> query_;
		};
	}
}

#endif