/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/traversable/traverse_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/traversable.hpp>

#include <tuple>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto traversable = detail::minimal::traversable<mcd>;
    constexpr auto applicative = detail::minimal::applicative<>;
    constexpr auto comparable = detail::minimal::comparable<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [=](auto x) {
        return applicative(std::make_tuple(x));
    };

    BOOST_HANA_CONSTEXPR_ASSERT(
        traverse<detail::minimal::Applicative<>>(f, traversable(comparable(1)))
            ==
        applicative(traversable(std::make_tuple(comparable(1))))
    );
}

int main() {
    test<Traversable::traverse_mcd>();
}
