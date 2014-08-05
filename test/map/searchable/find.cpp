/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/product.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <int i>
constexpr auto key = int_<i>;

template <int i>
constexpr auto value = int_<i + 100>;

template <int i, int j>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = detail::minimal::product<>(key<i>, value<j>);

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

BOOST_HANA_CONSTANT_ASSERT(find(map(), is(key<1>)) == nothing);

BOOST_HANA_CONSTANT_ASSERT(find(map(p<1, 1>), is(key<1>)) == just(value<1>));
BOOST_HANA_CONSTANT_ASSERT(find(map(p<1, 1>), is(key<2>)) == nothing);

BOOST_HANA_CONSTANT_ASSERT(find(map(p<1, 1>, p<2, 2>), is(key<1>)) == just(value<1>));
BOOST_HANA_CONSTANT_ASSERT(find(map(p<1, 1>, p<2, 2>), is(key<2>)) == just(value<2>));
BOOST_HANA_CONSTANT_ASSERT(find(map(p<1, 1>, p<2, 2>), is(key<3>)) == nothing);

int main() { (void)is; }
