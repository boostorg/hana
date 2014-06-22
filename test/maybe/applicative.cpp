/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(lift<Maybe>(1) == just(1));
    BOOST_HANA_STATIC_ASSERT(lift<Maybe>('1') == just('1'));
    BOOST_HANA_STATIC_ASSERT(lift<Maybe>(1.1) == just(1.1));

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return just(x + 1); };
    constexpr auto x = 3;
    BOOST_HANA_STATIC_ASSERT(ap(nothing, nothing) == nothing);
    BOOST_HANA_STATIC_ASSERT(ap(just(f), nothing) == nothing);
    BOOST_HANA_STATIC_ASSERT(ap(nothing, just(x)) == nothing);
    BOOST_HANA_STATIC_ASSERT(ap(just(f), just(x)) == just(f(x)));
}
