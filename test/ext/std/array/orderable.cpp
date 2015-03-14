/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/orderable.hpp>

#include <array>
using namespace boost::hana;


template <int ...i>
constexpr auto array = std::array<int, sizeof...(i)>{{i...}};

int main() {
    auto int_arrays = make<Tuple>(
          array<>
        , array<0>
        , array<0, 1>
        , array<0, 1, 2>
        , array<0, 1, 2, 3>
        , array<0, 1, 2, 3, 4>
    );

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    test::TestOrderable<ext::std::Array>{int_arrays};
}
