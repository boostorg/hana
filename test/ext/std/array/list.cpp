/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/array.hpp>

#include <boost/hana/detail/assert.hpp>

#include <array>
using namespace boost::hana;


template <int ...i>
auto array = std::array<int, sizeof...(i)>{{i...}};

int main() {
    // nil
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<StdArray>, std::array<char, 0>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<StdArray>, std::array<int, 0>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<StdArray>, std::array<long, 0>{}));
    }

    // cons
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<>), array<0>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<1>), array<0, 1>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<1, 2>), array<0, 1, 2>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<1, 2, 3>), array<0, 1, 2, 3>));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, nil<StdArray>), array<0>));
    }
}
