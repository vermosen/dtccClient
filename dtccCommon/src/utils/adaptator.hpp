#ifndef UTILS_ADAPTATOR_HPP_
#define UTILS_ADAPTATOR_HPP_

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/spirit/include/qi.hpp>

#include "database/record/enum.hpp"
#include "application/logger.hpp"

typedef boost::tuple<int, int, int, int, int, int> timeAdaptator;
typedef boost::optional<timeAdaptator> optTimeAdaptator;
typedef boost::tuple<int, int, int> dateAdaptator;
typedef boost::optional<dateAdaptator> optDateAdaptator;
typedef std::string assetTypeAdaptator;
typedef std::string severityAdaptator;

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

template<>
struct boost::spirit::traits::transform_attribute<dtcc::database::assetType, assetTypeAdaptator, boost::spirit::qi::domain>
{
	typedef assetTypeAdaptator type;

	static type pre(dtcc::database::assetType a) { return type(); }

	static void post(dtcc::database::assetType& d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::database::assetType>::toEnum(v);
	}

	static void fail(dtcc::database::assetType&) {}
};

template<>
struct boost::spirit::traits::transform_attribute<dtcc::severity, severityAdaptator, boost::spirit::qi::domain>
{
	typedef severityAdaptator type;

	static type pre(dtcc::severity a) { return type(); }

	static void post(dtcc::severity& d, type const& v)
	{
		d = dtcc::EnumManager<dtcc::severity>::toEnum(v);
	}

	static void fail(dtcc::severity&) {}
};

#endif