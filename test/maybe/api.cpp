/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) {
    return x + int_<1>;
};

int main() {
    // is_just
    BOOST_HANA_STATIC_ASSERT(is_just(just(1)));
    BOOST_HANA_STATIC_ASSERT(!is_just(nothing));

    // is_nothing
    BOOST_HANA_STATIC_ASSERT(!is_nothing(just(1)));
    BOOST_HANA_STATIC_ASSERT(is_nothing(nothing));

    // from_maybe
    BOOST_HANA_STATIC_ASSERT(from_maybe('d', nothing) == 'd');
    BOOST_HANA_STATIC_ASSERT(from_maybe('d', just(1)) == 1);
    BOOST_HANA_STATIC_ASSERT(from_maybe('d', just(int_<3>)) == int_<3>);

    // from_just
    BOOST_HANA_STATIC_ASSERT(from_just(just(1)) == 1);
    // from_just(nothing);

    // maybe
    BOOST_HANA_STATIC_ASSERT(maybe(int_<0>, inc, nothing) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(maybe(int_<0>, inc, just(int_<1>)) == int_<2>);
}
