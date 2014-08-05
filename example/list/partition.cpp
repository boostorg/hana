/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
using namespace boost::hana;


//! [ints]
BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % int_<2> != int_<0>;
};

BOOST_HANA_CONSTANT_ASSERT(
    partition(integer_list<int, 1, 2, 3, 4, 5, 6, 7>, odd)
    ==
    pair(
        integer_list<int, 1, 3, 5, 7>,
        integer_list<int, 2, 4, 6>
    )
);
//! [ints]

//! [types]
BOOST_HANA_CONSTANT_ASSERT(
    partition(type_list<void, int, float, char, double>, trait<std::is_floating_point>)
    ==
    pair(
        type_list<float, double>,
        type_list<void, int, char>
    )
);
//! [types]

int main() { (void)odd; }
