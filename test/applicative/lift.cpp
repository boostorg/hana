/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/applicative.hpp>

#include <boost/hana/detail/minimal/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    constexpr auto applicative = detail::minimal::applicative<mcd>;
    using A = detail::minimal::Applicative<mcd>;

    BOOST_HANA_STATIC_ASSERT(lift<A>(x<0>) == applicative(x<0>));
    BOOST_HANA_STATIC_ASSERT(lift<A>(x<1>) == applicative(x<1>));
}

int main() {
    test<Applicative::mcd>();
}
