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
	{
		io_ = boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);
		dt_ = boost::gregorian::day_clock::universal_day();
	}

	worker::~worker() { io_.reset(); };

	void worker::work()
	{
		launchDebugger();

		// initialize and run the service
		ioTask_ = boost::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*io_));

		// runs in a separate thread
		t_ = std::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		// build the connection
		// TODO: use the settings + factories
		cnx_ = boost::shared_ptr<connection>(new https(io_,
			connectionDelegate(boost::bind(&worker::connect_callback, this, boost::placeholders::_1)), true));
		cnx_->connect(settings_.connector_.host_, settings_.connector_.port_);

		// barrier
		boost::unique_lock<boost::mutex> lk(m_);
		while (!terminate_) cv_.wait(lk);
		ioTask_.reset();
		t_->join();
	}

	std::string worker::filename()
	{
		// https://kgc0418-tdw-data2-0.s3.amazonaws.com/slices/SLICE_COMMODITIES_2017_05_04_1.zip

		// parse the file name
		// TODO: parse the date and add a date counter
		std::stringstream ss;

		auto * temp(new boost::gregorian::date_facet("%Y_%m_%d"));
		ss.imbue(std::locale(ss.getloc(), temp));

		ss	<< "slices/SLICE_"
			<< settings_.description_.fileStr_
			<< "_"
			<< dt_
			<< "_"
			<< boost::lexical_cast<std::string>(counter_)
			<< ".zip";

		return ss.str();
	}

	void worker::connect_callback(bool result)
	{
		if (result == true)
		{
			LOG_INFO() << "host " + cnx_->host() + " successfully reached";

			reader_ = boost::shared_ptr<reader>(new reader(cnx_,
				urlReadDelegate(boost::bind(&worker::reader_callback, this, 
					boost::placeholders::_1, boost::placeholders::_2))));

			boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterSuccess_));
			filename_ = filename();
			reader_->getAsync(filename_);
		}
		else // abort or retry ?
		{
			LOG_ERROR() << "cannot connect to host " << settings_.connector_.host_;
		}
	}

	void worker::reader_callback(std::string msg, bool result)
	{
		try
		{
			if (result)
			{
				counter_++;
				LOG_INFO() << "successfully retrived data from " << filename_;

				// unzipping the records
				dtcc::archive<dtcc::zip::zip> ar(std::move(msg));

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

						boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resolution_clock::now();

						std::vector<dtcc::database::tradeRecord> recs;

						if (dtcc::parser::parseRecords(file.begin(), file.end(), recs, dt_))
						{
							LOG_INFO() << recs.size() << " conversions done in "
								<< boost::chrono::duration_cast<boost::chrono::milliseconds> (
									boost::chrono::high_resolution_clock::now() - start);

							start = boost::chrono::high_resolution_clock::now();

							write_(recs);
						}
						else
						{
							LOG_WARNING() << "failed to extract data from file " << filename_;
						}
					}

					filename_ = filename();
					boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterSuccess_));
					reader_->getAsync(filename_);
				}
			}
			else
			{
				throw std::exception();
			}
		}
		catch (std::exception & ex)
		{
			if (nFailure_ < settings_.maxAttempt_)
			{
				nFailure_++;
				LOG_WARNING() << "failed to retrive data from " << filename_;
				boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterFailure_));
				reader_->getAsync(filename_);
			}
			else
			{
				// TODO: we wat to continue the loop, but we 
				// check first if there have been a date change
				LOG_INFO() << "max attempt reached, checking setup...";
				reader_->getAsync(filename_);
			}
		}
	}
}