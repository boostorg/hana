/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/product.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
constexpr auto key = detail::minimal::comparable<>(i);

template <int i>
constexpr auto value = detail::minimal::comparable<>(i + 100);

template <int i, int j>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = detail::minimal::product<>(key<i>, value<j>);

int main() {
    BOOST_HANA_CONSTANT_ASSERT(keys(map()) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(keys(map(p<1, 1>)) == list(key<1>));
    BOOST_HANA_CONSTEXPR_ASSERT(keys(map(p<1, 1>, p<2, 2>)) == list(key<1>, key<2>));
    BOOST_HANA_CONSTEXPR_ASSERT(keys(map(p<1, 1>, p<2, 2>, p<3, 3>)) == list(key<1>, key<2>, key<3>));
}
