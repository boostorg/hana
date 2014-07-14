/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


constexpr auto applicative = detail::minimal::applicative<>;

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return std::make_tuple(x);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return applicative(g(x));
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    using A = detail::minimal::Applicative<>;
    BOOST_HANA_STATIC_ASSERT(traverse<A>(f, just(x<0>)) == applicative(just(g(x<0>))));
    BOOST_HANA_STATIC_ASSERT(traverse<A>(f, nothing) == applicative(nothing));
}
