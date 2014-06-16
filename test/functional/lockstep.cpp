/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return std::make_tuple(xs...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return x + 1; };
BOOST_HANA_CONSTEXPR_LAMBDA auto h = g;
BOOST_HANA_CONSTEXPR_LAMBDA auto i = g;

int main() {
    BOOST_HANA_STATIC_ASSERT(lockstep(f)() == f());
    BOOST_HANA_STATIC_ASSERT(lockstep(f, g)(1) == f(g(1)));
    BOOST_HANA_STATIC_ASSERT(lockstep(f, g, h)(1, '2') == f(g(1), h('2')));
    BOOST_HANA_STATIC_ASSERT(lockstep(f, g, h, i)(1, '2', 3.3) == f(g(1), h('2'), i(3.3)));
}
