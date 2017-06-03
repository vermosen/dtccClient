#include "appRunner.hpp"

namespace dtcc
{
	appRunner::appRunner(const dtcc::settings & settings) : settings_(settings)
	{
		LOG_INFO() << "Trying to connect to sql server";
		db_.reset(new dtcc::database::sqlServer());	// TODO: get the db type from the settings
		io_.reset(new boost::asio::io_service);
	}

	appRunner::~appRunner() { io_.reset(); }

	void appRunner::run()
	{
		// connect the database
		db_->connect(settings_.database_);

		// start the io service
		ioTask_.reset(new boost::asio::io_service::work(*io_));
		t_ = std::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		// build the protocol object
		cnx_.reset(new dtcc::web::https(io_, dtcc::web::connectionDelegate(
			boost::bind(&appRunner::connect_callback, this, boost::asio::placeholders::error))));
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
			rd_.reset(new dtcc::web::asio(cnx_, dtcc::web::urlReadDelegate(
				boost::bind(&appRunner::load_callback, this, boost::asio::placeholders::error, boost::lambda::_2))));

			// main loop
			for (auto dt = settings_.startDate_; dt <= settings_.endDate_; dt += boost::gregorian::date_duration(1))
			{
				LOG_INFO() << "Start activity on " << boost::gregorian::to_simple_string(dt);

				for (auto it = settings_.assets_.cbegin(); it != settings_.assets_.cend(); it++)
				{
					rd_->getAsync(boost::shared_ptr<dtcc::web::query>(new dtcc::web::eod(dt, *it)));			// get async
				}

				// add a day
				dt += boost::gregorian::date_duration(1);
			}
		}
		else 
		{
			LOG_ERROR() << err.category().name() << ":" << err.value();
			finished_ = true;
		}
	}
	void appRunner::load_callback(const boost::system::error_code& err, std::string data)
	{
		if (!err)
		{
			dtcc::database::tradeRecordset rs(db_);

			// TODO: error check
			std::vector<dtcc::database::tradeRecord> recs;			// data buffer

			dtcc::archive<dtcc::zip::zip> ar(std::move(data));

			if (!ar.open())
			{
				LOG_ERROR() << "failed to open the archive ";
			}
			else
			{
				auto fs = ar.fileSystem();

				for (auto jt = fs.begin(); jt != fs.end(); jt++)
				{
					std::string file = ar.get(*jt).str();

					LOG_INFO() << "Zip extraction successfull...";

					start_ = boost::chrono::high_resolution_clock::now();

					LOG_INFO() << "Starting record conversion...";

					auto qr = boost::static_pointer_cast<dtcc::web::eod>(rd_->getQuery());

					if (dtcc::parser::parseRecords(file.begin(), file.end(), recs, qr->date()))
					{
						LOG_INFO() << recs.size() << " conversions done in "
							<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
								boost::chrono::high_resolution_clock::now() - start_);

						start_ = boost::chrono::high_resolution_clock::now();

						// date facet
						std::ostringstream os;
						auto * temp(new boost::gregorian::date_facet("%Y-%m-%d"));
						os.imbue(std::locale(os.getloc(), temp));
						os << qr->date();

						std::string id = boost::lexical_cast<std::string>(static_cast<int>(qr->asset().type_));
						std::string filter = "FILE_DATE = '" + os.str() + "' AND ASSET_CLASS = " + id;

						rs.remove(filter);

						start_ = boost::chrono::high_resolution_clock::now();

						LOG_INFO() << "Trades cleanup done in "
							<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
								boost::chrono::high_resolution_clock::now() - start_);

						start_ = boost::chrono::high_resolution_clock::now();

						auto ids = rs.insert(recs);
						recs.clear();

						LOG_INFO() << ids.size() << " insertions done in "
							<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
								boost::chrono::high_resolution_clock::now() - start_);
					}
					else
					{
						LOG_ERROR() << "An error has occurred while converting the records";
						finished_ = true;
					}
				}
			}
		}
		else
		{
			LOG_ERROR() << err.category().name() << ":" << err.value();
			finished_ = true;
		}
	}
}