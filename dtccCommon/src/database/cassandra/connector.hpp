#ifndef CONNECTORS_CASSANDRA_HPP_
#define CONNECTORS_CASSANDRA_HPP_

#include <vector>

#include <cassandra.h>

#include "database/connector.hpp"

namespace dtcc
{
	namespace database
	{
		namespace cassandra
		{
			class connector;

			class cluster
			{
				friend connector;
			public:
				cluster(const std::vector<std::string> & nodes)
					: nodes_(nodes)
				{
					cluster_ = cass_cluster_new();

					for(auto & i : nodes_)
						cass_cluster_set_contact_points(cluster_, i.c_str());
				}

				~cluster() 
				{
					cass_cluster_free(cluster_);
				}

			private:
				std::vector<std::string> nodes_;
				CassCluster * cluster_;
			};

			class connector : public database::connector
			{
			public:
				connector();
				~connector()
				{
					cass_session_free(session_);
				}
				virtual void connect();
				virtual void close();
				virtual void begin() {}
				virtual void commit() {}
				virtual void rollback() {}
				virtual void execute(const std::string &);

				void setCluster(const boost::shared_ptr<cluster> & cluster);

				static registerTypeNoArgs<database::connector, std::string, connector> register_;

			private:
				boost::shared_ptr<cluster> cluster_;
				CassSession * session_;
			};
		}
	}
}
#endif /* CONNECTORS_CASSANDRA_HPP_ */