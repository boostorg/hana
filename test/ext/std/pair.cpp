/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/pair.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/orderable.hpp>
#include <laws/product.hpp>

#include <utility>
using namespace boost::hana;


using test::ct_eq;
using test::ct_ord;

int main() {
    auto eq_elems = make<tuple_tag>(ct_eq<3>{}, ct_eq<4>{});

    auto eqs = make<tuple_tag>(
          std::make_pair(ct_eq<3>{}, ct_eq<3>{})
        , std::make_pair(ct_eq<3>{}, ct_eq<4>{})
        , std::make_pair(ct_eq<4>{}, ct_eq<3>{})
        , std::make_pair(ct_eq<4>{}, ct_eq<4>{})
    );

    auto ords = make<tuple_tag>(
          std::make_pair(ct_ord<3>{}, ct_ord<3>{})
        , std::make_pair(ct_ord<3>{}, ct_ord<4>{})
        , std::make_pair(ct_ord<4>{}, ct_ord<3>{})
        , std::make_pair(ct_ord<4>{}, ct_ord<4>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable, Orderable, Foldable
    //////////////////////////////////////////////////////////////////////////
    test::TestComparable<ext::std::pair_tag>{eqs};
    test::TestOrderable<ext::std::pair_tag>{ords};
    test::TestFoldable<ext::std::pair_tag>{eqs};

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    {
        // first
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                first(std::make_pair(ct_eq<0>{}, ct_eq<1>{})),
                ct_eq<0>{}
            ));
        }

        // second
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                second(std::make_pair(ct_eq<0>{}, ct_eq<1>{})),
                ct_eq<1>{}
            ));
        }

        // make
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<ext::std::pair_tag>(ct_eq<0>{}, ct_eq<1>{}),
                std::make_pair(ct_eq<0>{}, ct_eq<1>{})
            ));
        }

        // laws
        test::TestProduct<ext::std::pair_tag>{eq_elems};
    }
}
