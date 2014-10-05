/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map/comparable.hpp>
#include <boost/hana/map/convert.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/minimal_product.hpp>
#include <test/minimal_record.hpp>
#include <test/seq/foldable.hpp>
#include <test/seq/list.hpp>
using namespace boost::hana;


template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto key = test::x<i>;

template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto value = test::x<-i>;

template <int i, int j>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = test::minimal_product<>(key<i>, value<j>);

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldable = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto record = test::minimal_record<>;
    using L = test::Seq;
    using test::x;

    // Record -> Map
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Map>(record(test::x<1>, test::x<2>)),
            map(pair(test::member1, test::x<1>),
                pair(test::member2, test::x<2>))
        ));
    }

    // Foldable -> Map
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Map>(foldable()),
            map()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Map>(foldable(p<1, 1>)),
            map(p<1, 1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Map>(foldable(p<1, 1>, p<2, 2>)),
            map(p<1, 1>, p<2, 2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Map>(foldable(p<1, 1>, p<2, 2>, p<3, 3>)),
            map(p<1, 1>, p<2, 2>, p<3, 3>)
        ));
    }

    // Map -> List
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto check = [=](auto ...xs) {
            BOOST_HANA_CONSTANT_ASSERT(
                elem(permutations(list(xs...)), to<L>(map(xs...)))
            );
        };
        check();
        check(p<1, 1>);
        check(p<1, 1>, p<2, 2>);
        check(p<1, 1>, p<2, 2>, p<3, 3>);
        check(p<1, 1>, p<2, 2>, p<3, 3>, p<4, 4>);
    }
}
