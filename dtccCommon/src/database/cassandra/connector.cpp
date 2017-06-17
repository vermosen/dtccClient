#include "connector.hpp"

namespace dtcc
{
	namespace database
	{
		namespace cassandra
		{
			//registerTypeNoArgs<database::connector, std::string, connector>
			//	connector::register_(std::string("cassandra"));

			connector::connector(const std::string & keyspace) 
				: database::connector()
				, keyspace_(keyspace)
			{
				session_ = cass_session_new();
			}

			void connector::connect()
			{
				future ft;
				ft.inst_ = cass_session_connect(session_, cluster_->cluster_);

				cass_future_wait(ft.inst_);
				CassError rc = cass_future_error_code(ft.inst_);
				cass_future_free(ft.inst_);

				if (rc != CASS_OK)
					throw std::exception("cannot connect to the cluster");

				// connect to the keyspace

			}

			void connector::close()
			{
				CassFuture * future = cass_session_close(session_);
				CassError rc = cass_future_error_code(future);
				cass_future_free(future);

				if (rc != CASS_OK)
				{
					// TODO: read error 
					throw std::exception();
				}
			}

			void connector::execute(const std::string & st)
			{
				CassFuture * future = NULL;
				CassStatement * statement = cass_statement_new(st.c_str(), 0);
				future = cass_session_execute(session_, statement);
				cass_future_wait(future);

				CassError rc = cass_future_error_code(future);
				if (rc != CASS_OK)
				{
					// TODO: read error 
					throw std::exception();
				}

				cass_future_free(future);
				cass_statement_free(statement);
			}

			void connector::setCluster(const boost::shared_ptr<cluster> & cluster) 
			{ 
				cluster_ = cluster; 
			}

			const std::string & connector::keyspace() const
			{
				return keyspace_;
			}
		}
	}
}