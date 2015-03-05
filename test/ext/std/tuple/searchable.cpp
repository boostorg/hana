/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          std::make_tuple()
        , std::make_tuple(eq<0>{})
        , std::make_tuple(eq<0>{}, eq<1>{})
        , std::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
    );

    auto eq_tuple_keys = make<Tuple>(
        eq<3>{}, eq<5>{}, eq<7>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestSearchable<ext::std::Tuple>{eq_tuples, eq_tuple_keys};
    }
}
