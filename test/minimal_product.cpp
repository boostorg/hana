/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <test/minimal_product.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/orderable.hpp>
#include <laws/product.hpp>
using namespace boost::hana;


using test::ct_eq;
using test::ct_ord;

int main() {
    // make sure `first` and `second` are "accessors"
    auto prod = test::minimal_product(test::Tracked{1}, test::Tracked{2});
    first(std::move(prod));
    second(std::move(prod));


    //////////////////////////////////////////////////////////////////////////
    // Comparable, Orderable, Foldable, Product
    //////////////////////////////////////////////////////////////////////////
    auto eq_elems = make<tuple_tag>(ct_eq<3>{}, ct_eq<4>{});

    auto eqs = make<tuple_tag>(
          test::minimal_product(ct_eq<3>{}, ct_eq<3>{})
        , test::minimal_product(ct_eq<3>{}, ct_eq<4>{})
        , test::minimal_product(ct_eq<4>{}, ct_eq<3>{})
        , test::minimal_product(ct_eq<4>{}, ct_eq<4>{})
    );

    auto ords = make<tuple_tag>(
          test::minimal_product(ct_ord<3>{}, ct_ord<3>{})
        , test::minimal_product(ct_ord<3>{}, ct_ord<4>{})
        , test::minimal_product(ct_ord<4>{}, ct_ord<3>{})
        , test::minimal_product(ct_ord<4>{}, ct_ord<4>{})
    );

    test::TestComparable<test::MinimalProduct>{eqs};
    test::TestOrderable<test::MinimalProduct>{ords};
    test::TestFoldable<test::MinimalProduct>{eqs};
    test::TestProduct<test::MinimalProduct>{eq_elems};
}
