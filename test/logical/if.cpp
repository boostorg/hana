/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/logical.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    constexpr auto logical = detail::minimal::logical<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(if_(logical(true), x<0>, x<1>) == x<0>);
    BOOST_HANA_CONSTEXPR_ASSERT(if_(logical(false), x<0>, x<1>) == x<1>);
}

int main() {
    test<Logical::mcd>();
}
