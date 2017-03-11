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
	dtcc::settings2,
	(boost::gregorian::date, startDate_)
	(boost::gregorian::date, endDate_)
	(std::string, baseUrl_)
	(int64_t, memory_)
	(std::vector<std::string>, assets_)
)

template<>
struct boost::spirit::traits::transform_attribute<boost::gregorian::date, dateAdaptator, boost::spirit::qi::domain>
{
	typedef dateAdaptator type;

	static type pre(boost::gregorian::date a) { return type(); }

	static void post(boost::gregorian::date& d, type const& v)
	{
		d = boost::gregorian::date(boost::get<0>(v), boost::get<1>(v), boost::get<2>(v));
	}

	static void fail(boost::gregorian::date&) {}
};

#endif