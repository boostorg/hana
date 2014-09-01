/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map/comparable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/laws/comparable.hpp>
#include <test/injection.hpp>
#include <test/minimal_product.hpp>
using namespace boost::hana;


template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto key = test::x<i>;

template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto value = test::x<-i>;

template <int i, int j>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = test::minimal_product<>(key<i>, value<j>);

int main() {
    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(map(), map()));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map())));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(), map(p<1, 1>))));

        BOOST_HANA_CONSTANT_ASSERT(equal(map(p<1, 1>), map(p<1, 1>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map(p<1, 2>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map(p<2, 1>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map(p<1, 1>, p<2, 2>))));

        BOOST_HANA_CONSTANT_ASSERT(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<2, 2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(map(p<1, 1>, p<2, 2>), map(p<2, 2>, p<1, 1>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<9, 1>, p<2, 2>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 9>, p<2, 2>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<9, 2>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<2, 9>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<2, 2>, p<3, 3>))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            map(),
            map(p<1, 1>),
            map(p<1, 2>),
            map(p<1, 1>, p<2, 2>)
        ));
    }
}
