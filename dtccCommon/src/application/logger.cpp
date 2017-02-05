#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/phoenix/bind/bind_function_object.hpp>

#include "application/logger.hpp"

namespace dtcc
{
	bool				logger::initialized_	= false				;
	logger::logger_t	logger::logger_			= logger::logger_t();
	severity			logger::threshold_		= severity::info	;

	template<typename C, typename T>
	inline std::basic_ostream<C, T> & operator << (std::basic_ostream<C, T>& strm, severity lvl)
	{
		static constexpr char * const str[] =
		{
			"debug",
			"info ",
			"warn ",
			"error",
			"fatal"
		};

		if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
			strm << str[lvl];
		else
			strm << static_cast<int>(lvl);
		return strm;
	}
		
	bool logger::levelFilter(boost::log::value_ref<severity, tag::severity_t> const & level)
	{
		return level >= threshold_;
	}

	logger::logger_t & logger::getLogger() { return logger_; }

	void logger::initialize(const std::string & fileName, severity threshold)
	{
		if (!logger::initialized_)
		{
			logger_		= logger_t()				;
			threshold_  = threshold					;
			auto c		= boost::log::core::get()	;

			c->add_global_attribute("TimeStamp", boost::log::attributes::utc_clock			());
			c->add_global_attribute("ProcessID", boost::log::attributes::current_process_id	());
			c->add_global_attribute("ThreadID" , boost::log::attributes::current_thread_id	());

			c->set_filter(boost::phoenix::bind(&levelFilter, severity_t.or_none()));

			boost::shared_ptr< boost::log::sinks::text_file_backend > backend =
				boost::make_shared<boost::log::sinks::text_file_backend>
				(
					boost::log::keywords::auto_flush			= false,
					boost::log::keywords::file_name				= fileName,
					boost::log::keywords::open_mode				= std::ios_base::app | std::ios_base::out,
					boost::log::keywords::rotation_size			= 10 * 1024 * 1024,											// 10 MB
					boost::log::keywords::time_based_rotation	= boost::log::sinks::file::rotation_at_time_point(0, 0, 0)
				);

			boost::shared_ptr<sink_t> sink(new sink_t(backend));

			sink->set_formatter
			(
				boost::log::expressions::stream
				<< "[" 
				<< boost::log::expressions::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") 
				<< "] <" 
				<< severity_t 
				<< "> "
				<< boost::log::expressions::smessage
			);

			c->add_sink(sink);
		}
	}
}
