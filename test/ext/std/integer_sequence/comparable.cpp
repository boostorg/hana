/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integer_sequence.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/comparable.hpp>
#include <utility>
using namespace boost::hana;


template <typename T, typename U>
void test() {
    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            std::integer_sequence<T>{},
            std::integer_sequence<U>{}
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            std::integer_sequence<T, 0>{},
            std::integer_sequence<U>{}
        )));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            std::integer_sequence<T>{},
            std::integer_sequence<U, 0>{}
        )));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            std::integer_sequence<T, 0>{},
            std::integer_sequence<U, 0>{}
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            std::integer_sequence<T, 0>{},
            std::integer_sequence<U, 0, 1>{}
        )));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            std::integer_sequence<T, 0, 2>{},
            std::integer_sequence<U, 0, 1>{}
        )));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            std::integer_sequence<T, 0, 1, 2, 3>{},
            std::integer_sequence<U, 0, 1, 2, 3>{}
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            std::integer_sequence<T, 0, 1, 2, 3, 5>{},
            std::integer_sequence<U, 0, 1, 2, 3>{}
        )));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            std::integer_sequence<T>{},
            std::integer_sequence<U>{},

            std::integer_sequence<T, 0>{},
            std::integer_sequence<U, 0>{},

            std::integer_sequence<T, 0, 1>{},
            std::integer_sequence<U, 0, 1>{},

            std::integer_sequence<T, 1, 0, 2>{},
            std::integer_sequence<U, 1, 0, 2>{}
        ));
    }
}

int main() {
    test<int, char>();
    test<int, int>();
    test<int, long>();
    test<int, unsigned long>();
}
