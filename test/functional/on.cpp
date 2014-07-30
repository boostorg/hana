/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...x) {
    return std::make_tuple('f', x...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return std::make_tuple('g', x);
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_STATIC_ASSERT(on(f, g)() == f());
    BOOST_HANA_STATIC_ASSERT(on(f, g)(x<0>) == f(g(x<0>)));
    BOOST_HANA_STATIC_ASSERT(on(f, g)(x<0>, x<1>) == f(g(x<0>), g(x<1>)));
    BOOST_HANA_STATIC_ASSERT(on(f, g)(x<0>, x<1>, x<2>) == f(g(x<0>), g(x<1>), g(x<2>)));
    BOOST_HANA_STATIC_ASSERT(on(f, g)(x<0>, x<1>, x<2>, x<3>) == f(g(x<0>), g(x<1>), g(x<2>), g(x<3>)));

    // check the infix version
    BOOST_HANA_STATIC_ASSERT((f /on/ g)() == f());
    BOOST_HANA_STATIC_ASSERT((f /on/ g)(x<0>) == f(g(x<0>)));
    BOOST_HANA_STATIC_ASSERT((f /on/ g)(x<0>, x<1>) == f(g(x<0>), g(x<1>)));
    BOOST_HANA_STATIC_ASSERT((f /on/ g)(x<0>, x<1>, x<2>) == f(g(x<0>), g(x<1>), g(x<2>)));
    BOOST_HANA_STATIC_ASSERT((f /on/ g)(x<0>, x<1>, x<2>, x<3>) == f(g(x<0>), g(x<1>), g(x<2>), g(x<3>)));
}
