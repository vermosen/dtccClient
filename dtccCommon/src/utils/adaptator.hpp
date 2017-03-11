#ifndef UTILS_ADAPTATOR_HPP_
#define UTILS_ADAPTATOR_HPP_

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>

typedef boost::tuple<int, int, int, int, int, int> timeAdaptator;
typedef boost::optional<timeAdaptator> optTimeAdaptator;
typedef boost::tuple<int, int, int> dateAdaptator;
typedef boost::optional<dateAdaptator> optDateAdaptator;

#endif