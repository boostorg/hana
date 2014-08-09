/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/identity/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>

#include <tuple>
using namespace boost::hana;


constexpr auto applicative = detail::identity<>;
using A = detail::Identity<>;

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return std::make_tuple(x);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return applicative(g(x));
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    BOOST_HANA_CONSTANT_ASSERT(
        equal(traverse<A>(f, list()),
        applicative(list())
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(
        equal(traverse<A>(f, list(x<0>)),
        applicative(list(g(x<0>)))
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(
        equal(traverse<A>(f, list(x<0>, x<1>)),
        applicative(list(g(x<0>), g(x<1>)))
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(
        equal(traverse<A>(f, list(x<0>, x<1>, x<2>)),
        applicative(list(g(x<0>), g(x<1>), g(x<2>)))
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(
        equal(traverse<A>(f, list(x<0>, x<1>, x<2>, x<3>)),
        applicative(list(g(x<0>), g(x<1>), g(x<2>), g(x<3>)))
    ));
}

int main() {
    test<List::mcd<void>>();
}
