/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integer_sequence.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/searchable.hpp>

#include <utility>
using namespace boost::hana;


using T = int;
using U = long long;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto sequences = make<Tuple>(
        std::integer_sequence<T>{},
        std::integer_sequence<T, 2>{},
        std::integer_sequence<U, 3>{},
        std::integer_sequence<T, 3, 4>{},
        std::integer_sequence<U, 3, 4, 5>{},
        std::integer_sequence<U, 4, 3, 5>{}
    );

    auto keys = make<Tuple>(
        integral_constant<T, 3>, integral_constant<U, 4>
    );

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // laws
        test::TestSearchable<ext::std::IntegerSequence>{sequences, keys};
    }
}
