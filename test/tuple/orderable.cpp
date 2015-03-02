/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/orderable.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

template <int i>
using ord = test::ct_ord<i>;

struct x0; struct x1; struct x2; struct x3;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(eq<0>{})
        , make<Tuple>(eq<0>{}, eq<1>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
    );

    auto ord_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(ord<0>{})
        , make<Tuple>(ord<0>{}, ord<1>{})
        , make<Tuple>(ord<0>{}, ord<1>{}, ord<2>{})
        , make<Tuple>(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{})
        , make<Tuple>(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{}, ord<4>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal

        // tuple_t
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<>, tuple_t<>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<>, tuple_t<x0>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0>, tuple_t<x0>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0, x1>, tuple_t<x0>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<x0, x1>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1>, tuple_t<x0, x1>));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1, x2>, tuple_t<x0, x1, x2>));

        // tuple_c
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int>, tuple_c<int>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_c<int, 0>, tuple_c<int>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0>, tuple_c<int, 0>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_c<int, 0, 1>, tuple_c<int, 0>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0, 1>, tuple_c<int, 0, 1>));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0, 1, 2>, tuple_c<int, 0, 1, 2>));

        test::TestComparable<Tuple>{eq_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestOrderable<Tuple>{ord_tuples};
    }
}
