/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


constexpr auto x = detail::number<>(0);
constexpr auto s = detail::number<>(1);
BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

int main() {
    // foldl
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(just(x), s, f), f(s, x)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(nothing, s, f), s));
    }

    // foldr
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(just(x), s, f), f(x, s)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(nothing, s, f), s));
    }
}
