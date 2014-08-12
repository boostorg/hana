/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <test/laws/constant.hpp>
using namespace boost::hana;


template <typename T>
void tests() {
    // value
    {
        static_assert(value(integral<T, 0>) == 0, "");
        static_assert(value(integral<T, 1>) == 1, "");
    }

    // laws
    {
        Constant_laws(
            integral<T, 0>,
            integral<T, 1>,
            integral<T, 2>,
            integral<T, 3>
        );
    }
}

int main() {
    tests<int>();
    tests<long>();
}
