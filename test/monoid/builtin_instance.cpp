/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monoid/monoid.hpp>

#include <boost/hana/core/datatype.hpp>
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

using Integer = datatype_t<integer>;
using Integer2 = datatype_t<integer2>;

int main() {
    // same type
    BOOST_HANA_CONSTEXPR_ASSERT(zero<Integer>.value == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{3}, integer{5}).value == 3 + 5);

    BOOST_HANA_CONSTEXPR_ASSERT(zero<Integer2>.value == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer2{3}, integer2{5}).value == 3 + 5);

    // mixed types
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{3}, integer2{5}).value == 3 + 5);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer2{3}, integer{5}).value == 3 + 5);
}
