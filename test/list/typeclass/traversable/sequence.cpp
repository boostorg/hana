/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
using namespace boost::hana;


constexpr auto applicative = detail::minimal::applicative<>;

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    using A = detail::minimal::Applicative<>;

    BOOST_HANA_CONSTANT_ASSERT(sequence<A>(list()) == applicative(list()));
    BOOST_HANA_CONSTEXPR_ASSERT(sequence<A>(list(applicative(x<0>))) == applicative(list(x<0>)));
    BOOST_HANA_CONSTEXPR_ASSERT(sequence<A>(list(applicative(x<0>), applicative(x<1>))) == applicative(list(x<0>, x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(sequence<A>(list(applicative(x<0>), applicative(x<1>), applicative(x<2>))) == applicative(list(x<0>, x<1>, x<2>)));
}

int main() {
    test<List::mcd<void>>();
}
