#pragma once
#ifndef HTTPS_HPP_
#define HTTPS_HPP_

#include <boost/asio/ssl.hpp>
#include <boost/lexical_cast.hpp>

#include "application/web/asio/protocol.hpp"

namespace dtcc
{
	namespace web
	{
		class https : public protocol
		{
		public:
			https(boost::shared_ptr<boost::asio::io_service> io, connectionDelegate cnx, bool verify = false)
				: protocol(io, cnx)
				, context_(boost::asio::ssl::context::sslv23)
				, socket_(*io_, context_)
				, verify_(verify)
			{
				context_.set_default_verify_paths();
			}

			const std::string & name() const { return "https"; }

			virtual void connect(const std::string & host, int port)
			{
				host_ = host; port_ = port;

				if (verify_)
				{
					// set verify mode
					socket_.set_verify_mode(boost::asio::ssl::verify_peer);

					// set the callbacks
					socket_.set_verify_callback(
						boost::bind(&https::handle_checkCertificate, 
							this, boost::placeholders::_1, boost::placeholders::_2));
				}
				else
				{
					socket_.set_verify_mode(boost::asio::ssl::verify_none);
				}


				auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
					new boost::asio::ip::tcp::resolver::query(
						host_, boost::lexical_cast<std::string>(port)));

				resolver_.async_resolve(*query,
					boost::bind(&https::handle_resolve, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::iterator));
			}

			virtual boost::asio::ip::tcp::socket & socket() { return socket_.next_layer(); };

			// callbacks
			bool https::handle_checkCertificate(bool preverified, boost::asio::ssl::verify_context& ctx)
			{
				char subject_name[256];
				X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
				X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
				return preverified;
			}

			void handle_resolve(const boost::system::error_code& err,
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
			{
				if (!err)
				{
					boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
						st_.wrap(boost::bind(&https::handle_connect, this,
							boost::asio::placeholders::error)));
				}
				else
				{
					// TODO: error handler
					cnx_(false);
				}
			}

		private:
			boost::asio::ssl::context context_;
			boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
			bool verify_;
		};
	}
}

#endif