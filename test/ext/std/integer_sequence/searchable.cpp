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
        std::integer_sequence<U>{},

        std::integer_sequence<T, 0>{},
        std::integer_sequence<U, 0>{},

        std::integer_sequence<T, 0, 1>{},
        std::integer_sequence<U, 0, 1>{},

        std::integer_sequence<T, 1, 0, 2>{},
        std::integer_sequence<U, 1, 0, 2>{}
    );

    auto keys = make<Tuple>(
        integral_constant<T, 0>, integral_constant<U, 1>
    );

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // laws
        test::TestSearchable<ext::std::IntegerSequence>{sequences, keys};
    }
}
