/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/traversable/traverse_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/identity/applicative.hpp>
#include <boost/hana/detail/identity/traversable.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto traversable = detail::identity<Mcd>;
    constexpr auto applicative = detail::identity<>;
    constexpr auto comparable = detail::number<>;
    using A = detail::Identity<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

    // sequence
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            sequence<A>(traversable(applicative(comparable(1)))),
            applicative(traversable(comparable(1)))
        ));
    }

    // traverse
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            traverse<A>(compose(lift<A>, f), traversable(comparable(1))),
            applicative(traversable(f(comparable(1))))
        ));
    }
}

int main() {
    test<Traversable::traverse_mcd>();
}
