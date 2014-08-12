/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/identity/applicative.hpp>
#include <test/identity/comparable.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto a = test::identity;
    using A = test::Identity;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    using test::x;

    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(a(f), a(x<0>)),
            a(f(x<0>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(a(f), a(x<0>), a(x<1>)),
            a(f(x<0>, x<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(a(f), a(x<0>), a(x<1>), a(x<2>)),
            a(f(x<0>, x<1>, x<2>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(a(f), a(x<0>), a(x<1>), a(x<2>), a(x<3>)),
            a(f(x<0>, x<1>, x<2>, x<3>))
        ));
    }

    // lift
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            lift<A>(x<0>),
            a(x<0>)
        ));
    }
}
