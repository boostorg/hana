/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});
BOOST_HANA_CONSTEXPR_LAMBDA auto h = detail::injection([]{});

template <int i>
constexpr auto x = detail::number<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        compose(f, g)(x<0>),
        f(g(x<0>))
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        compose(f, g)(x<0>, x<1>),
        f(g(x<0>), x<1>)
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        compose(f, g)(x<0>, x<1>, x<2>),
        f(g(x<0>), x<1>, x<2>)
    ));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        compose(f, g, h)(x<0>),
        f(g(h(x<0>)))
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        compose(f, g, h)(x<0>, x<1>),
        f(g(h(x<0>)), x<1>)
    ));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        compose(f, g, h)(x<0>, x<1>, x<2>),
        f(g(h(x<0>)), x<1>, x<2>)
    ));
}
