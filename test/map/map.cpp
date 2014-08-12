/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/minimal_list.hpp>
#include <test/minimal_product.hpp>
using namespace boost::hana;


template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto key = test::x<i>;

template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto value = test::x<-i>;

template <int i, int j>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = test::minimal_product<>(key<i>, value<j>);

int main() {
    constexpr auto list = test::minimal_list<>;

    // keys
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            keys(map()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            keys(map(p<1, 1>)),
            list(key<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            keys(map(p<1, 1>, p<2, 2>)),
            list(key<1>, key<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            keys(map(p<1, 1>, p<2, 2>, p<3, 3>)),
            list(key<1>, key<2>, key<3>)
        ));
    }

    // values
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            values(map()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            values(map(p<1, 1>)),
            list(value<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            values(map(p<1, 1>, p<2, 2>)),
            list(value<1>, value<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            values(map(p<1, 1>, p<2, 2>, p<3, 3>)),
            list(value<1>, value<2>, value<3>)
        ));
    }
}
