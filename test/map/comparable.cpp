/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <test/minimal_product.hpp>
using namespace boost::hana;


template <int i>
auto key = test::ct_eq<i>{};

template <int i>
auto val = test::ct_eq<-i>{};

template <int i, int j>
auto p = test::minimal_product(key<i>, val<j>);


int main() {
    auto eq_maps = make<Tuple>(
        make<Map>(),
        make<Map>(p<1, 1>),
        make<Map>(p<1, 2>),
        make<Map>(p<1, 1>, p<2, 2>),
        make<Map>(p<1, 1>, p<2, 2>, p<3, 3>)
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(),
                make<Map>()
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(),
                make<Map>(p<1, 1>)
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<1, 1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<1, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<2, 1>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<1, 1>, p<2, 2>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<2, 2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<2, 2>, p<1, 1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<9, 1>, p<2, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 9>, p<2, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<9, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<2, 9>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<2, 2>, p<3, 3>))
            ));
        }

        // laws
        test::TestComparable<Map>{eq_maps};
    }
}
