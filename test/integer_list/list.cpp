/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integer_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <test/laws/list.hpp>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    // cons
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(integral<T, 0>, integer_list<U>),
            integer_list<U, 0>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(integral<T, 0>, integer_list<U, 1>),
            integer_list<U, 0, 1>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(integral<T, 0>, integer_list<U, 1, 2>),
            integer_list<U, 0, 1, 2>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(integral<T, 0>, integer_list<U, 1, 2, 3>),
            integer_list<U, 0, 1, 2, 3>
        ));
    }

    // nil
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<IntegerList>, integer_list<T>));
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<IntegerList>, integer_list<U>));
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<IntegerList>, integer_list<void>));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(List_laws(
            integer_list<T>, integer_list<T, 0>, integer_list<T, 0, 1>,
            integer_list<U>, integer_list<U, 0>, integer_list<U, 0, 1>
        ));
    }
}

int main() {
    tests<int, int>();
    tests<int, unsigned int>();

    tests<int, long>();
    tests<int, unsigned long>();
}
