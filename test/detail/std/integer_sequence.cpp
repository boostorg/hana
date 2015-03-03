/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/std/integer_sequence.hpp>

#include <type_traits>
using namespace boost::hana;


static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 0>,
    detail::std::integer_sequence<int>
>::value, "");

static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 1>,
    detail::std::integer_sequence<int, 0>
>::value, "");

static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 2>,
    detail::std::integer_sequence<int, 0, 1>
>::value, "");

static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 3>,
    detail::std::integer_sequence<int, 0, 1, 2>
>::value, "");

static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 4>,
    detail::std::integer_sequence<int, 0, 1, 2, 3>
>::value, "");

static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 5>,
    detail::std::integer_sequence<int, 0, 1, 2, 3, 4>
>::value, "");

static_assert(std::is_same<
    detail::std::make_integer_sequence<int, 6>,
    detail::std::integer_sequence<int, 0, 1, 2, 3, 4, 5>
>::value, "");


int main() { }
