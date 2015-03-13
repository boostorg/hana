/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>
#include <test/minimal_product.hpp>
#include <test/seq.hpp>
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

    auto eq_keys = make<Tuple>(key<1>, key<4>);

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(), equal.to(key<1>)))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>), equal.to(key<1>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(p<1, 1>), equal.to(key<2>)))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<1>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<2>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<3>)))
            );
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(), equal.to(key<1>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>), equal.to(key<1>)),
                just(val<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>), equal.to(key<2>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<1>)),
                just(val<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<2>)),
                just(val<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<3>)),
                nothing
            ));
        }

        // laws
        test::TestSearchable<Map>{eq_maps, eq_keys};
    }
}
