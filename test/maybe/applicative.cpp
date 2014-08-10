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


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
constexpr auto x = detail::number<>(0);

int main() {
    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT( equal(ap(nothing, nothing), nothing));
        BOOST_HANA_CONSTANT_ASSERT( equal(ap(just(f), nothing), nothing));
        BOOST_HANA_CONSTANT_ASSERT( equal(ap(nothing, just(x)), nothing));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(ap(just(f), just(x)), just(f(x))));
    }

    // lift
    {
        BOOST_HANA_CONSTEXPR_ASSERT(lift<Maybe>(x) == just(x));
    }
}
