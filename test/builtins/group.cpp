/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/group/group.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


struct integer {
    int value;
    constexpr explicit integer(int i) : value{i} { }
};

struct integer2 {
    int value;
    constexpr explicit integer2(int i) : value{i} { }
};

constexpr auto operator+(integer a, integer b)
{ return integer{a.value + b.value}; }

constexpr auto operator+(integer2 a, integer b)
{ return integer{a.value + b.value}; }

constexpr auto operator+(integer a, integer2 b)
{ return integer{a.value + b.value}; }

constexpr auto operator+(integer2 a, integer2 b)
{ return integer2{a.value + b.value}; }


constexpr auto operator-(integer a, integer b)
{ return integer{a.value - b.value}; }

constexpr auto operator-(integer2 a, integer b)
{ return integer{a.value - b.value}; }

constexpr auto operator-(integer a, integer2 b)
{ return integer{a.value - b.value}; }

constexpr auto operator-(integer2 a, integer2 b)
{ return integer2{a.value - b.value}; }

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(negate(integer{3}).value == -3);

    // same type
    BOOST_HANA_CONSTEXPR_ASSERT(minus(integer{3}, integer{5}).value == 3 - 5);

    // // mixed types
    BOOST_HANA_CONSTEXPR_ASSERT(minus(integer{3}, integer2{5}).value == 3 - 5);
    BOOST_HANA_CONSTEXPR_ASSERT(minus(integer2{3}, integer{5}).value == 3 - 5);
}
