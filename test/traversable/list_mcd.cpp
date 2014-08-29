/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>

#include <test/identity/applicative.hpp>
#include <test/identity/comparable.hpp>
#include <test/injection.hpp>
#include <test/minimal_list.hpp>
using namespace boost::hana;


template <typename mcd>
void tests() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = test::minimal_list<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto applicative = test::identity;
    using A = test::Identity;
    using test::x;

    // traverse
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(list(), compose(applicative, f)),
            applicative(list())
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(list(x<0>), compose(applicative, f)),
            applicative(list(f(x<0>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(list(x<0>, x<1>), compose(applicative, f)),
            applicative(list(f(x<0>), f(x<1>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(list(x<0>, x<1>, x<2>), compose(applicative, f)),
            applicative(list(f(x<0>), f(x<1>), f(x<2>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(list(x<0>, x<1>, x<2>, x<3>), compose(applicative, f)),
            applicative(list(f(x<0>), f(x<1>), f(x<2>), f(x<3>)))
        ));
    }

    // sequence
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sequence<A>(list()),
            applicative(list())
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sequence<A>(list(applicative(x<0>))),
            applicative(list(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sequence<A>(list(applicative(x<0>), applicative(x<1>))),
            applicative(list(x<0>, x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sequence<A>(list(applicative(x<0>), applicative(x<1>), applicative(x<2>))),
            applicative(list(x<0>, x<1>, x<2>))
        ));
    }
}

int main() {
    tests<List::mcd<void>>();
}
