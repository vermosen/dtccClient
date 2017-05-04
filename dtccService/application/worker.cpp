#include "worker.hpp"

namespace dtcc
{
	worker::worker(settings::worker & settings, writeDelegate write, const std::string name)
		: workerBase(name)
		, settings_(settings)
		, write_(write)
		, counter_(1)
		, nFailure_(0)
	{
		io_ = boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);
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
			connectionDelegate(boost::bind(&worker::connect_callback, this, _1)), true));
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
		std::stringstream ss;

		ss	<< "slices/SLICE_"
			<< settings_.description_.fileStr_
			<< "_"
			<< "2017_05_04"
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
				urlReadDelegate(boost::bind(&worker::reader_callback, this, _1, _2))));

			boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterSuccess_));
			filename_ = filename();
			reader_->getAsync(filename_);
		}
		else // abort or retry ?
		{
			LOG_ERROR() << "cannot connect to host " << settings_.connector_.host_;
		}
	}

	void worker::reader_callback(const std::string & msg, bool result)
	{
		if (result)
		{
			counter_++;
			LOG_INFO() << "successfully retrived data from " << filename_;
			write_(msg); filename_ = filename();
			boost::this_thread::sleep(boost::posix_time::milliseconds(settings_.timeoutAfterSuccess_));
			reader_->getAsync(filename());
		}
		else
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
				LOG_WARNING() << "max attempt reached, aborting task...";
			}
			
		}
	}
}