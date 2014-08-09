/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_domain/integral_domain.hpp>

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

constexpr auto operator/(integer a, integer b)
{ return integer{a.value / b.value}; }

constexpr auto operator/(integer2 a, integer b)
{ return integer{a.value / b.value}; }

constexpr auto operator/(integer a, integer2 b)
{ return integer{a.value / b.value}; }

constexpr auto operator/(integer2 a, integer2 b)
{ return integer2{a.value / b.value}; }


constexpr auto operator%(integer a, integer b)
{ return integer{a.value % b.value}; }

constexpr auto operator%(integer2 a, integer b)
{ return integer{a.value % b.value}; }

constexpr auto operator%(integer a, integer2 b)
{ return integer{a.value % b.value}; }

constexpr auto operator%(integer2 a, integer2 b)
{ return integer2{a.value % b.value}; }

int main() {
    // same type
    BOOST_HANA_CONSTEXPR_ASSERT(quot(integer{6}, integer{3}).value == 6 / 3);
    BOOST_HANA_CONSTEXPR_ASSERT(mod(integer{6}, integer{4}).value == 6 % 4);

    // mixed types
    BOOST_HANA_CONSTEXPR_ASSERT(quot(integer{6}, integer2{3}).value == 6 / 3);
    BOOST_HANA_CONSTEXPR_ASSERT(quot(integer2{6}, integer{3}).value == 6 / 3);

    BOOST_HANA_CONSTEXPR_ASSERT(mod(integer{6}, integer2{4}).value == 6 % 4);
    BOOST_HANA_CONSTEXPR_ASSERT(mod(integer2{6}, integer{4}).value == 6 % 4);
}
