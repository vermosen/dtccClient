#include "appRunner.hpp"

namespace dtcc
{
	appRunner::appRunner(const dtcc::settings & settings)
		: settings_(settings)
		, it_(settings_.assets_.cbegin())
		, dt_(settings_.startDate_)
	{
		LOG_INFO() << "Trying to connect to sql server";
		db_.reset(new dtcc::database::sql::connector());	// TODO: get the db type from the settings
		io_.reset(new boost::asio::io_service);
	}

	appRunner::~appRunner() { io_.reset(); }

	void appRunner::run()
	{
		db_->setConnectionString(settings_.database_);
		// connect the database
		db_->connect();

		// start the io service
		ioTask_.reset(new boost::asio::io_service::work(*io_));
		t_ = std::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		// build the protocol object
		cnx_.reset(new dtcc::web::asio::https(io_, dtcc::web::connectionDelegate(
			boost::bind(&appRunner::connect_callback, this, boost::asio::placeholders::error)), false, true));

		cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);

		// barrier
		boost::unique_lock<boost::mutex> lk(m_);
		while (!finished_)
		{
			cv_.wait(lk);
		}

		ioTask_.reset();
		t_->join();
	}

	void appRunner::connect_callback(const boost::system::error_code& err)
	{
		if (!err)
		{
			// build the reader object	
			rd_.reset(new dtcc::web::asio::reader(cnx_, dtcc::web::urlReadDelegate(
				boost::bind(&appRunner::load_callback, this, boost::asio::placeholders::error, boost::lambda::_2))));

			LOG_INFO()	<< "Start activity on " 
						<< boost::gregorian::to_simple_string(settings_.startDate_) 
						<< " and asset " 
						<< it_->fileStr_;

			rd_->getAsync(boost::shared_ptr<dtcc::web::query>(new dtcc::web::eod(dt_, *it_)));
		}
		else 
		{
			LOG_ERROR() << err.category().name() << ":" << err.value();
			finished_ = true;
			cv_.notify_one();
		}
	}
	void appRunner::load_callback(const boost::system::error_code& err, const dtcc::web::content & ct)
	{
		if (!err || err == boost::asio::error::eof)
		{
			if (ct.stream_.str().size() != 0)
			{
				dtcc::database::sql::tradeRecordset rs(db_);

				// TODO: error check
				std::vector<dtcc::database::tradeRecord> recs;			// data buffer

				dtcc::archive<dtcc::zip::zip> ar(std::move(ct.stream_.str()));

				if (!ar.open())
				{
					LOG_ERROR()
						<< "failed to open the archive ";
				}
				else
				{
					auto fs = ar.fileSystem();

					for (auto jt = fs.begin(); jt != fs.end(); jt++)
					{
						std::string file = ar.get(*jt);

						LOG_INFO()
							<< "Zip extraction successful...";

						start_ = boost::chrono::high_resolution_clock::now();

						LOG_INFO()
							<< "Starting record conversion...";

						auto qr = boost::static_pointer_cast<dtcc::web::eod>(rd_->getQuery());

						if (dtcc::parser::parseRecords(file.begin(), file.end(), recs, qr->date()))
						{
							LOG_INFO()
								<< recs.size() << " conversions done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start_);

							start_ = boost::chrono::high_resolution_clock::now();

							// date facet
							std::ostringstream os;
							auto * temp(new boost::gregorian::date_facet("%Y-%m-%d"));
							os.imbue(std::locale(os.getloc(), temp));

							// filter
							os << "FILE_DATE = '"
								<< qr->date()
								<< "' AND ASSET_CLASS = "
								<< boost::lexical_cast<std::string>(static_cast<int>(qr->asset().type_));

							rs.remove(os.str());

							start_ = boost::chrono::high_resolution_clock::now();

							LOG_INFO()
								<< "Trades cleanup done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start_);

							start_ = boost::chrono::high_resolution_clock::now();

							auto ids = rs.insert(recs);
							recs.clear();

							LOG_INFO() << ids.size() << " insertions done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start_);

							// next iteration
							if (++it_ == settings_.assets_.cend())
							{
								it_ = settings_.assets_.cbegin();
								dt_ += boost::gregorian::days(1);
							}

							if (dt_ <= settings_.endDate_)
							{
								LOG_INFO()
									<< "Start activity on date "
									<< boost::gregorian::to_simple_string(qr->date())
									<< " and asset "
									<< it_->fileStr_;

								if (!err)
								{
									// get
									rd_->getAsync(boost::shared_ptr<dtcc::web::query>(new dtcc::web::eod(dt_, *it_)));
								}
								else
								{
									LOG_WARNING() << "connection has been closed by the distant server. ";

									// we need to reconnect to the server first
									cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);
								}
							}
							else
							{
								LOG_INFO() << "All the activity successfully finished !";
								finished_ = true;
								cv_.notify_one();
							}
						}
						else
						{
							LOG_ERROR() << "An error has occurred while converting the records";
							finished_ = true;
							cv_.notify_one();
						}
					}
				}
			}
			else // EOF raised during the reconnection
			{
				LOG_WARNING() << "connection has been closed by the distant server. ";
				cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);
			}			
		}
		else 
		{
			LOG_ERROR() << err.category().name() << ":" << err.value();
			finished_ = true;
			cv_.notify_one();
		}
	}
}