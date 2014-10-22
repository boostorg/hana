/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/core.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/char.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/size_t.hpp>
#include <type_traits>
using namespace boost::hana;


static_assert(std::is_same<
    datatype_t< ::boost::mpl::bool_<true>>,
    ext::boost::mpl::IntegralC
>::value, "");

static_assert(std::is_same<
    datatype_t< ::boost::mpl::int_<0>>,
    ext::boost::mpl::IntegralC
>::value, "");

static_assert(std::is_same<
    datatype_t< ::boost::mpl::long_<0>>,
    ext::boost::mpl::IntegralC
>::value, "");

static_assert(std::is_same<
    datatype_t< ::boost::mpl::size_t<0>>,
    ext::boost::mpl::IntegralC
>::value, "");

static_assert(std::is_same<
    datatype_t< ::boost::mpl::integral_c<int, 0>>,
    ext::boost::mpl::IntegralC
>::value, "");

static_assert(std::is_same<
    datatype_t< ::boost::mpl::char_<0>>,
    ext::boost::mpl::IntegralC
>::value, "");

int main() { }
