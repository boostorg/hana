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

constexpr auto x = detail::number<>;

int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(on(f, g)(), f()));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(on(f, g)(x(0)), f(g(x(0)))));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(on(f, g)(x(0), x(1)), f(g(x(0)), g(x(1)))));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(on(f, g)(x(0), x(1), x(2)), f(g(x(0)), g(x(1)), g(x(2)))));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(on(f, g)(x(0), x(1), x(2), x(3)), f(g(x(0)), g(x(1)), g(x(2)), g(x(3)))));

    // check the infix version
    BOOST_HANA_CONSTANT_ASSERT(equal((f ^on^ g)(), f()));
    BOOST_HANA_CONSTEXPR_ASSERT(equal((f ^on^ g)(x(0)), f(g(x(0)))));
    BOOST_HANA_CONSTEXPR_ASSERT(equal((f ^on^ g)(x(0), x(1)), f(g(x(0)), g(x(1)))));
    BOOST_HANA_CONSTEXPR_ASSERT(equal((f ^on^ g)(x(0), x(1), x(2)), f(g(x(0)), g(x(1)), g(x(2)))));
    BOOST_HANA_CONSTEXPR_ASSERT(equal((f ^on^ g)(x(0), x(1), x(2), x(3)), f(g(x(0)), g(x(1)), g(x(2)), g(x(3)))));
}
