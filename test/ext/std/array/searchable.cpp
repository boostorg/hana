/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>

#include <array>
using namespace boost::hana;


using test::ct_eq;

int main() {
    auto eq_arrays = make<Tuple>(
          std::array<ct_eq<0>, 0>{}
        , std::array<ct_eq<0>, 1>{}
        , std::array<ct_eq<0>, 2>{}
        , std::array<ct_eq<0>, 3>{}
        , std::array<ct_eq<0>, 4>{}
    );

    auto eq_keys = make<Tuple>(ct_eq<0>{});

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    test::TestSearchable<ext::std::Array>{eq_arrays, eq_keys};
}
