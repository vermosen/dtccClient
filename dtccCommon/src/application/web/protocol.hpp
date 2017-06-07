#pragma once
#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "utils/thread.hpp"

namespace dtcc
{
	namespace web
	{
		typedef boost::function<void(const boost::system::error_code&)> connectionDelegate;

		class protocol
		{
		public:
			enum type
			{
				http	= 1,
				https	= 2
			};

			protocol(type type, const connectionDelegate & dlg, bool keepAlive) : dlg_(dlg) {}

			void connect(const std::string & host, int port)
			{
				host_ = host; port_ = port;
				this->connectImpl();
			}

			virtual void connectImpl() = 0;

			void handle_connect(const boost::system::error_code& err)
			{
				if (!err)
				{
					dlg_(boost::system::errc::make_error_code(boost::system::errc::success));
				}
				else
				{
					dlg_(err);
				}
			}

			// accessors
			const std::string & host() const { return host_; }
			int port() const { return port_; }
			type name() const { return type_; }
			bool keepAlive() const { return keepAlive_; }

		protected:
			connectionDelegate dlg_;
			std::string host_;
			int port_;
			type type_;
			bool keepAlive_;
		};

		namespace asio
		{
			class protocol : public web::protocol
			{
			public:

				protocol(web::protocol::type type, boost::shared_ptr<boost::asio::io_service> io, const connectionDelegate & dlg, bool keepAlive)
					: web::protocol(type, dlg, keepAlive)
					, io_(io)
					, st_(*io_)
					, resolver_(*io_) {}

				virtual boost::asio::ip::tcp::socket & socket() = 0;
				boost::shared_ptr<boost::asio::io_service> io_service() { return io_; };
				boost::asio::strand	& strand() { return st_; };

			protected:
				boost::shared_ptr<boost::asio::io_service> io_;
				boost::asio::ip::tcp::resolver resolver_;
				boost::asio::strand st_;
			};
		}
	}
}


#endif