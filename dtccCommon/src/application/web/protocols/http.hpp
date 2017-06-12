#pragma once
#ifndef HTTP_HPP_
#define HTTP_HPP_

#include <boost/lexical_cast.hpp>

#include "application/web/protocol.hpp"

namespace dtcc
{
	namespace web
	{
		namespace asio
		{
			class http : public protocol
			{
			public:
				http(boost::shared_ptr<boost::asio::io_service> io, const connectionDelegate & dlg, bool keepAlive = false)
					: protocol(web::protocol::type::https, io, dlg, keepAlive)
					, socket_(*io_) {}

				virtual void connectImpl()
				{
					auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
						new boost::asio::ip::tcp::resolver::query(
							host_, boost::lexical_cast<std::string>(port_)));

					resolver_.async_resolve(*query, st_.wrap(
						boost::bind(&http::handle_resolve, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::iterator)));
				}

				virtual boost::asio::ip::tcp::socket & socket() { return socket_; };

			protected:
				void handle_resolve(const boost::system::error_code& err,
					boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
				{
					if (!err)
					{
						boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
							st_.wrap(boost::bind(&http::handle_connect, this,
								boost::asio::placeholders::error)));
					}
					else
					{
						dlg_(err);
					}
				}

				boost::asio::ip::tcp::socket socket_;
			};
		}
	}
}

#endif