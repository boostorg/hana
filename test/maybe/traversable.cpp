/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>

#include <test/identity/applicative.hpp>
#include <test/identity/comparable.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto applicative = test::identity;
    using A = test::Identity;
    using test::x;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // traverse
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(compose(applicative, f), just(x<0>)),
            applicative(just(f(x<0>)))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(compose(applicative, f), nothing),
            applicative(nothing)
        ));
    }
}
