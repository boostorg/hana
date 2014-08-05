/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integer_sequence.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>

#include <utility>
using namespace boost::hana;


template <typename T, typename U>
void test() {
    BOOST_HANA_CONSTANT_ASSERT(
        Comparable::laws::check(
            list(
                std::integer_sequence<T>{},
                std::integer_sequence<U>{},

                std::integer_sequence<T, 0>{},
                std::integer_sequence<U, 0>{},

                std::integer_sequence<T, 0, 1>{},
                std::integer_sequence<U, 0, 1>{},

                std::integer_sequence<T, 1, 0, 2>{},
                std::integer_sequence<U, 1, 0, 2>{}
            )
        )
    );
}

int main() {
    test<int, int>();
    test<int, unsigned long long>();
}
