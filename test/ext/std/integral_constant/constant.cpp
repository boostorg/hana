/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/constant.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename T>
void tests() {
    // value
    {
        static_assert(value(std::integral_constant<T, 0>{}) == 0, "");
        static_assert(value(std::integral_constant<T, 1>{}) == 1, "");
    }

    // laws
    {
        Constant_laws(
            std::integral_constant<T, 0>{},
            std::integral_constant<T, 1>{},
            std::integral_constant<T, 2>{},
            std::integral_constant<T, 3>{}
        );
    }
}

int main() {
    tests<int>();
    tests<long>();
}