/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/applicative/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/identity/applicative.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto a = detail::identity<Mcd>;
    using A = detail::Identity<Mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    constexpr auto c = detail::number<>; // comparable

    // ap
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ap(a(f), a(c(0))),
            a(f(c(0)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ap(a(f), a(c(0)), a(c(1))),
            a(f(c(0), c(1)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ap(a(f), a(c(0)), a(c(1)), a(c(2))),
            a(f(c(0), c(1), c(2)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ap(a(f), a(c(0)), a(c(1)), a(c(2)), a(c(3))),
            a(f(c(0), c(1), c(2), c(3)))
        ));
    }

    // lift
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            lift<A>(c(0)),
            a(c(0))
        ));
    }
}

int main() {
    test<Applicative::mcd>();
}
