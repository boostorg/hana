/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_ORDERABLE_LESS_MCD_MINIMAL_HPP
#define BOOST_HANA_TEST_ORDERABLE_LESS_MCD_MINIMAL_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/orderable.hpp>


struct MinimalOrderable;

template <int i>
struct _orderable {
    using hana_datatype = MinimalOrderable;

    // Comparison is required for the unit tests.
    template <int j>
    constexpr auto operator==(_orderable<j>)
    { return boost::hana::bool_<i == j>; }
};

template <int i>
constexpr _orderable<i> orderable{};

namespace boost { namespace hana {
    template <>
    struct Orderable::instance<MinimalOrderable, MinimalOrderable>
        : Orderable::less_mcd
    {
        template <int i, int j>
        static constexpr auto less_impl(_orderable<i>, _orderable<j>)
        { return bool_<(i < j)>; }
    };
}}

#endif // !BOOST_HANA_TEST_ORDERABLE_LESS_MCD_MINIMAL_HPP
