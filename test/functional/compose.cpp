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
    return std::make_tuple(1, x...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto ...x) {
    return std::make_tuple(2, x...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto h = [](auto ...x) {
    return std::make_tuple(3, x...);
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(x<0>) == f(g(x<0>)));
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(x<0>, x<1>) == f(g(x<0>), x<1>));
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(x<0>, x<1>, x<2>) == f(g(x<0>), x<1>, x<2>));

    BOOST_HANA_STATIC_ASSERT(compose(f, g, h)(x<0>) == f(g(h(x<0>))));
    BOOST_HANA_STATIC_ASSERT(compose(f, g, h)(x<0>, x<1>) == f(g(h(x<0>)), x<1>));
    BOOST_HANA_STATIC_ASSERT(compose(f, g, h)(x<0>, x<1>, x<2>) == f(g(h(x<0>)), x<1>, x<2>));
}
