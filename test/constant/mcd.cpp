/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>

#include <test/cnumeric/constant.hpp>
#include <test/laws/constant.hpp>
using namespace boost::hana;


template <typename T>
void tests() {
    // value
    {
        static_assert(value(test::cnumeric<T, 0>) == 0, "");
        static_assert(value(test::cnumeric<T, 1>) == 1, "");
    }

    // laws
    {
        Constant_laws(
            test::cnumeric<T, 0>,
            test::cnumeric<T, 1>,
            test::cnumeric<T, 2>,
            test::cnumeric<T, 3>
        );
    }
}

int main() {
    tests<int>();
    tests<long>();
}
