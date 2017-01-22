/*
* logger.hpp
*
*  Created on: Sep 25, 2016
*      Author: vermosen
*/

#ifndef APPLICATION_LOGGER_HPP_
#define APPLICATION_LOGGER_HPP_

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/phoenix/bind/bind_function_object.hpp>

using namespace boost::log;

namespace dtcc
{
	namespace logger
	{
		enum type : short
		{
			Debug	= 0,
			Info	= 1,
			Warning = 2,
			Error	= 3
		};
	}
}

namespace
{
	// define the keywords to use in filter and formatter
	BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", dtcc::logger::type)
	BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", attributes::utc_clock::value_type)
	BOOST_LOG_ATTRIBUTE_KEYWORD(processid, "PID", attributes::current_process_id::value_type)
	BOOST_LOG_ATTRIBUTE_KEYWORD(threadid, "Thread", attributes::current_thread_id::value_type)
}

namespace dtcc
{
	namespace logger
	{
		// logger type with severity level and multithread support
		typedef sources::severity_logger_mt<type> logger_t;

		// textfile sink with synchronization support
		typedef sinks::synchronous_sink<sinks::text_file_backend> sink_t;

		static type threshold_;
		static boost::shared_ptr<logger_t> logger_;

		// the filter used to filter log records according to level
		static bool levelFilter(value_ref< type, tag::severity> const& level)
		{
			return level >= threshold_;
		}

		static void setLogger(const std::string & fileName, type threshold = Warning)
		{
			// TODO Auto-generated constructor stub
			// create a logger
			logger_ = boost::shared_ptr<logger_t>(new logger_t());

			threshold_ = threshold;

			auto c = boost::log::core::get();

			// add attributes
			c->add_global_attribute("TimeStamp", attributes::utc_clock());
			c->add_global_attribute("ProcessID", attributes::current_process_id());
			c->add_global_attribute("ThreadID", attributes::current_thread_id());

			// add level filter
			c->set_filter(boost::phoenix::bind(&levelFilter, severity.or_none()));

			// create a sink with max size and automatic rotation
			boost::shared_ptr< sinks::text_file_backend > backend =
				boost::make_shared<sinks::text_file_backend>
				(
					//keywords::target = "localhost",
					keywords::auto_flush			= false,
					keywords::file_name				= fileName,						// filename
					keywords::open_mode				= std::ios_base::app | std::ios_base::out,
					keywords::rotation_size			= 10 * 1024 * 1024,			// 10 MB
					keywords::time_based_rotation	= sinks::file::rotation_at_time_point(0, 0, 0)
				);

			// autoflush
			//backend->auto_flush(true);

			// createe the actual sink
			boost::shared_ptr<sink_t> sink(new sink_t(backend));

			// sink formatter
			sink->set_formatter
			(
				expressions::stream
				<< "[" << expressions::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S") << "]"
				<< " <" << severity << "> "
				<< expressions::smessage
			);

			// add sink
			c->add_sink(sink);
		}

		// stream output support for log_level
		static std::ostream& operator<<(std::ostream& stream, type level)
		{
			static const char * strings[] =
			{
				"debug ",
				" info ",
				" warn ",
				"error "
			};

			unsigned int l = static_cast<size_t>(level);

			if (l >= 0 && l < sizeof(strings) / sizeof(strings[0]))
			{
				stream << strings[l];
			}
			else
			{
				stream << l;
			}

			return stream;
		}
	};
} /* namespace dtcc */

namespace
{
	// macros to get log streams
#define LOG_DEBUG()     BOOST_LOG_STREAM_SEV(*dtcc::logger::logger_, dtcc::logger::Debug) \
		<< __FUNCTION__ << "(): "
#define LOG_INFO()      BOOST_LOG_STREAM_SEV(*dtcc::logger::logger_, dtcc::logger::Info) \
		<< __FUNCTION__ << "(): "
#define LOG_WARNING()   BOOST_LOG_STREAM_SEV(*dtcc::logger::logger_, dtcc::logger::Warning) \
		<< __FUNCTION__ << "(): "
#define LOG_ERROR()     BOOST_LOG_STREAM_SEV(*dtcc::logger::logger_, dtcc::logger::Error) \
		<< __FUNCTION__ << "(): "
#define LOG()           LOG_INFO()
}

#endif /* APPLICATION_LOGGER_HPP_ */
