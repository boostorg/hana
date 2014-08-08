/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>

#include "../comparable.hpp"
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(equal(flatten(lazy(lazy(x<0>))), lazy(x<0>)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(flatten(lazy(lazy(x<1>))), lazy(x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(flatten(lazy(lazy(lazy(x<1>)))), lazy(lazy(x<1>))));
}
