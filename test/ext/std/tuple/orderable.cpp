/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/orderable.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

template <int i>
using ord = test::ct_ord<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          std::make_tuple()
        , std::make_tuple(eq<0>{})
        , std::make_tuple(eq<0>{}, eq<1>{})
        , std::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
        , std::make_tuple(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
    );

    auto ord_tuples = make<Tuple>(
          std::make_tuple()
        , std::make_tuple(ord<0>{})
        , std::make_tuple(ord<0>{}, ord<1>{})
        , std::make_tuple(ord<0>{}, ord<1>{}, ord<2>{})
        , std::make_tuple(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{}, ord<4>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable and Orderable
    //////////////////////////////////////////////////////////////////////////
    test::TestComparable<ext::std::Tuple>{eq_tuples};
    test::TestOrderable<ext::std::Tuple>{ord_tuples};
}
