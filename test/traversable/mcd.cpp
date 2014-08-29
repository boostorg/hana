/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/traversable/traverse_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>

#include <test/identity/applicative.hpp>
#include <test/identity/comparable.hpp>
#include <test/injection.hpp>
#include <test/seq/comparable.hpp>
#include <test/seq/traversable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto traversable = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto applicative = test::identity;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    using A = test::Identity;
    using test::x;

    // sequence
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sequence<A>(traversable(applicative(x<0>))),
            applicative(traversable(x<0>))
        ));
    }

    // traverse
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(traversable(x<0>), compose(lift<A>, f)),
            applicative(traversable(f(x<0>)))
        ));
    }
}
