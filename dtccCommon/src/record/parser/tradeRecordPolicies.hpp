#ifndef TRADE_RECORD_POLICIES_HPP_
#define TRADE_RECORD_POLICIES_HPP_

#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/optional/optional_io.hpp>

using namespace boost::spirit;

// currency amount policy
template <typename T>
struct currencyPolicy : boost::spirit::qi::real_policies<T>
{
	//  No exponent
	template <typename iterator>
	static bool
		parse_exp(iterator&, iterator const&)
	{
		return false;
	}

	//  No exponent
	template <typename iterator, typename Attribute>
	static bool
		parse_exp_n(iterator&, iterator const&, Attribute&)
	{
		return false;
	}

	//  Thousands separated numbers
	template <typename iterator, typename Attribute>
	static bool
		parse_n(iterator& first, iterator const& last, Attribute& attr)
	{
		using boost::spirit::qi::uint_parser;
		namespace qi = boost::spirit::qi;

		uint_parser<unsigned, 10, 1, 3> uint3;
		uint_parser<unsigned, 10, 3, 3> uint3_3;

		T result = 0;
		if (parse(first, last, uint3, result))
		{
			T n; iterator save = first;
			while (qi::parse(first, last, ",") && qi::parse(first, last, uint3_3, n))
			{
				result = result * 1000 + n;
				save = first;
			}

			first = save;
			attr = result;
			return true;
		}
		return false;
	}
};

struct boolPolicy
{
	template <typename iterator, typename Attribute>
	static bool
		parse_true(iterator& first, iterator const& last, Attribute& attr)
	{
		if (qi::detail::string_parse("Y", first, last, qi::unused))
		{
			traits::assign_to(true, attr);    // result is true
			return true;
		}
		return qi::bool_policies<bool>::parse_true(first, last, attr);
	}

	template <typename iterator, typename Attribute>
	static bool
		parse_false(iterator& first, iterator const& last, Attribute& attr)
	{
		if (qi::detail::string_parse("N", first, last, qi::unused))
		{
			traits::assign_to(false, attr);   // result is false
			return true;
		}
		return qi::bool_policies<bool>::parse_false(first, last, attr);
	}
};

#endif