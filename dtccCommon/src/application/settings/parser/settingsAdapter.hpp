#ifndef SETTINGS_ADAPTER_HPP_
#define SETTINGS_ADAPTER_HPP_

#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tuple/tuple.hpp>

#include "application/settings.hpp"
#include "utils/adaptator.hpp"

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::settings::logger,
	(std::string, fileStr_)
	(dtcc::severity, severity_)
)

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::settings::asset,
	(dtcc::database::assetType, type_)
	(std::string, fileStr_)
)

BOOST_FUSION_ADAPT_STRUCT(
	dtcc::settings,
	(dtcc::settings::logger, logger_)
	(std::string, database_)
	(boost::gregorian::date, startDate_)
	(boost::gregorian::date, endDate_)
	(std::string, baseUrl_)
	(int64_t, memory_)
	(std::vector<dtcc::settings::asset>, assets_)
)

#endif