/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map/searchable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

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
    BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
        return [=](auto y) { return equal(x, y); };
    };

    // any
    {
        BOOST_HANA_CONSTANT_ASSERT(not_(any(map(), is(key<1>))));

        BOOST_HANA_CONSTANT_ASSERT(any(map(p<1, 1>), is(key<1>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(any(map(p<1, 1>), is(key<2>))));

        BOOST_HANA_CONSTANT_ASSERT(any(map(p<1, 1>, p<2, 2>), is(key<1>)));
        BOOST_HANA_CONSTANT_ASSERT(any(map(p<1, 1>, p<2, 2>), is(key<2>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(any(map(p<1, 1>, p<2, 2>), is(key<3>))));
    }

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(map(), is(key<1>)),
            nothing
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(map(p<1, 1>), is(key<1>)),
            just(value<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(map(p<1, 1>), is(key<2>)),
            nothing
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(map(p<1, 1>, p<2, 2>), is(key<1>)),
            just(value<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(map(p<1, 1>, p<2, 2>), is(key<2>)),
            just(value<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(map(p<1, 1>, p<2, 2>), is(key<3>)),
            nothing
        ));
    }
}
