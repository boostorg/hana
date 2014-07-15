/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/traversable/traverse_mcd.hpp>

#include <boost/hana/detail/minimal/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/traversable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto traversable = detail::minimal::traversable<mcd>;
    constexpr auto applicative = detail::minimal::applicative<>;
    constexpr auto comparable = detail::minimal::comparable<>;

    BOOST_HANA_STATIC_ASSERT(
        sequence<detail::minimal::Applicative<>>(traversable(applicative(comparable(1))))
            ==
        applicative(traversable(comparable(1)))
    );
}

int main() {
    test<Traversable::traverse_mcd>();
}
