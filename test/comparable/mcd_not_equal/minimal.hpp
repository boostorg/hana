/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_COMPARABLE_MCD_NOT_EQUAL_MINIMAL_HPP
#define BOOST_HANA_TEST_COMPARABLE_MCD_NOT_EQUAL_MINIMAL_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>


struct MinimalComparable;

template <int i>
struct _comparable { using hana_datatype = MinimalComparable; };

template <int i>
constexpr _comparable<i> comparable{};

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<MinimalComparable, MinimalComparable>
        : Comparable::not_equal_mcd
    {
        template <int i, int j>
        static constexpr auto not_equal_impl(_comparable<i>, _comparable<j>)
        { return bool_<i != j>; }
    };
}}

#endif // !BOOST_HANA_TEST_COMPARABLE_MCD_NOT_EQUAL_MINIMAL_HPP
