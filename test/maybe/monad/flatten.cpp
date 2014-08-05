/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_CONSTANT_ASSERT(flatten(nothing) == nothing);
    BOOST_HANA_CONSTANT_ASSERT(flatten(just(nothing)) == nothing);
    BOOST_HANA_CONSTEXPR_ASSERT(flatten(just(just(x<0>))) == just(x<0>));
}
