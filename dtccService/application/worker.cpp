#include "worker.hpp"

#include "application/compression/zip.hpp"
#include "application/compression/archive.hpp"
#include "database/record/parser/parseRecords.hpp"

namespace dtcc
{
	worker::worker(settings::worker & settings, writeRecordsDelegate write, const std::string name)
		: workerBase	(name		)
		, settings_		(settings	)
		, write_		(write		)
		, counter_		(1			)
		, nFailure_		(0			)
		, terminate_	(false		)
	{
		io_.reset(new boost::asio::io_service);
		dt_ = boost::gregorian::day_clock::universal_day();
	}

	worker::~worker() { io_.reset(); };

	void worker::work()
	{
		// initialize and run the service
		LOG_INFO() << "starting new io task...";

		ioTask_.reset(new boost::asio::io_service::work(*io_));

		// runs in a separate thread
		t_ = std::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		// build the connection
		// TODO: use the settings + factories
		LOG_INFO() << "building new connector...";

		cnx_ = boost::shared_ptr<web::protocol>(new web::https(io_,
			web::connectionDelegate(boost::bind(&worker::connect_callback, this, boost::placeholders::_1)), true));

		cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);

		// barrier
		boost::unique_lock<boost::mutex> lk(m_);
		while (!terminate_) cv_.wait(lk);

		// wait x seconds before returning
		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

		// not clear
		ioTask_.reset();
		t_->join();
	}

	void worker::stop()
	{
		terminate_ = true;
		cv_.notify_one();
	}

	void worker::connect_callback(const boost::system::error_code& err)
	{
		if (!err)
		{
			cnx_->socket().set_option(boost::asio::socket_base::keep_alive(true));

			LOG_INFO() << "host " + cnx_->host() + " successfully reached";

			reader_.reset(new dtcc::web::asio::reader(cnx_,
				web::urlReadDelegate(boost::bind(&worker::reader_callback, this,
					boost::asio::placeholders::error, boost::lambda::_2))));

			boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterSuccess_));
			qr_ = boost::shared_ptr<web::intraday>(new web::intraday(dt_, settings_.description_));

			reader_->getAsync(qr_);
		}
		else // abort or retry ?
		{
			LOG_ERROR() << "cannot connect to host " << settings_.connector_.host_;
		}
	}

	void worker::reader_callback(const boost::system::error_code& err, const dtcc::web::content & ct)
	{
		try
		{
			if (!err)
			{
				counter_++;
				LOG_INFO() << "successfully retrived data from " << filename_;

				// unzipping the records
				dtcc::archive<dtcc::zip::zip> ar(std::move(ct.stream_.str()));

				if (!ar.open())
				{
					LOG_ERROR() << "failed to open the archive ";
				}
				else
				{
					auto fs = ar.fileSystem();

					LOG_INFO() << "found " << fs.size() << " items in the archive";

					for (auto jt = fs.begin(); jt != fs.end(); jt++)
					{
						std::string file = ar.get(*jt).str();

						LOG_INFO() << "Zip extraction successfull...";

						//boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now();

						std::vector<dtcc::database::tradeRecord> recs;

						if (dtcc::parser::parseRecords(file.begin(), file.end(), recs, dt_))
						{
							write_(recs);
						}
						else
						{
							LOG_WARNING() << "failed to extract data from file " << filename_;
						}
					}

					if (terminate_)
					{
						return;
					}
					else
					{
						// cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);
						// submit a new qr
						
						reader_->getAsync(qr_);

					}
				}
			}
			else if(err == boost::asio::error::connection_aborted)
			{
				// we just reset the connection and try again
				boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterFailure_));
				cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);
			}
			else
			{
				// we reset the connection
				throw std::exception();
			}
		}
		catch (std::exception & ex)
		{
			if (nFailure_ < settings_.maxAttempt_)
			{
				nFailure_++;
				LOG_WARNING() << "failed to retrive data from " << filename_;

				if (terminate_)
				{
					return;
				}
				else
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterFailure_));
					reader_->getAsync(qr_);
				}
			}
			else
			{
				// TODO: we wat to continue the loop, but we 
				// check first if there have been a date change
				LOG_INFO() << "max attempt reached, checking setup...";

				if (terminate_)
				{
					return;
				}
				else
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterFailure_));
					reader_->getAsync(qr_);
				}
			}
		}
	}
}