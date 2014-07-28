/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/minimal/product.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto key = detail::minimal::comparable<>(i);

template <int i>
constexpr auto value = detail::minimal::comparable<>(i + 100);

template <int i, int j>
constexpr auto p = detail::minimal::product<>(key<i>, value<j>);

int main() {
    using L = detail::minimal::List<>;
    constexpr auto list = detail::minimal::list<>;
    BOOST_HANA_STATIC_ASSERT(to<L>(map()) == list());
    BOOST_HANA_STATIC_ASSERT(to<L>(map(p<1, 1>)) == list(p<1, 1>));

    BOOST_HANA_STATIC_ASSERT(elem(to<L>(map(p<1, 1>, p<2, 2>)), permutations(list(p<1, 1>, p<2, 2>))));
    BOOST_HANA_STATIC_ASSERT(elem(to<L>(map(p<1, 1>, p<2, 2>, p<3, 3>)), permutations(list(p<1, 1>, p<2, 2>, p<3, 3>))));
}
