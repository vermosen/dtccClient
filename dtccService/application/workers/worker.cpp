#include "worker.hpp"

namespace dtcc
{
	worker::worker(writeDelegate write, const std::string name)
		: workerBase(name)
		, write_(write)
		, counter_(1)
	{
		io_ = boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);
	}

	worker::~worker() { io_.reset(); };

	void worker::work()
	{
		// initialize and run the service
		ioTask_ = boost::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*io_));

		// runs in a separate thread
		t_ = std::unique_ptr<boost::thread>(new boost::thread([&] { io_->run(); }));
		setThreadName(t_->get_id(), "io runner");

		cnx_ = boost::shared_ptr<connection>(new https(io_,
			connectionDelegate(boost::bind(&worker::connect_callback, this, _1)), true));
		cnx_->connect("kgc0418-tdw-data-0.s3.amazonaws.com", 80);

		// barrier
		boost::unique_lock<boost::mutex> lk(m_);
		while (!terminate_) cv_.wait(lk);
		ioTask_.reset();
		t_->join();
	}

	void worker::connect_callback(bool result)
	{
		if (result == true)
		{
			write_("host " + cnx_->host() + " successfully reached");

			reader_ = boost::shared_ptr<reader>(new reader(cnx_,
				urlReadDelegate(boost::bind(&worker::reader_callback, this, _1, _2))));
			reader_->getAsync("slices/SLICE_COMMODITIES_2017_04_03_129.zip");
		}
		else // abort or retry ?
		{
			write_("cannot connect to host...");
		}
	}

	void worker::reader_callback(const std::string & msg, bool result)
	{
		if (result)
		{
			counter_++;
			write_("successfully retrieved: " + msg);
			reader_->getAsync("");
		}
		else
		{
			write_("cannot connect to host...");
		}
	}
}