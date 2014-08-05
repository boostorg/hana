/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

int main() {
    BOOST_HANA_CONSTANT_ASSERT(!any(is(x<1>), set()));

    BOOST_HANA_CONSTEXPR_ASSERT( any(is(x<1>), set(x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(!any(is(x<2>), set(x<1>)));

    BOOST_HANA_CONSTEXPR_ASSERT( any(is(x<1>), set(x<1>, x<2>)));
    BOOST_HANA_CONSTEXPR_ASSERT( any(is(x<2>), set(x<1>, x<2>)));
    BOOST_HANA_CONSTEXPR_ASSERT(!any(is(x<3>), set(x<1>, x<2>)));
}
