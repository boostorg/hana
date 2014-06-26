/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include "minimal.hpp"
#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return std::make_tuple(1, xs...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto ...xs) {
    return std::make_tuple(2, xs...);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(lift<MinimalList>(1) == minimal_list(1));
    BOOST_HANA_STATIC_ASSERT(lift<MinimalList>(1.1) == minimal_list(1.1));
    BOOST_HANA_STATIC_ASSERT(lift<MinimalList>('1') == minimal_list('1'));

    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(), minimal_list()) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(), minimal_list(1)) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(), minimal_list(1, '2')) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(), minimal_list(1, '2', 3.3)) == minimal_list());

    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f), minimal_list()) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f), minimal_list(1)) == minimal_list(f(1)));
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f), minimal_list(1, '2')) == minimal_list(f(1), f('2')));
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f), minimal_list(1, '2', 3.3)) == minimal_list(f(1), f('2'), f(3.3)));

    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f, g), minimal_list()) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f, g), minimal_list(1)) == minimal_list(f(1), g(1)));
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f, g), minimal_list(1, '2')) == minimal_list(f(1), f('2'), g(1), g('2')));
    BOOST_HANA_STATIC_ASSERT(ap(minimal_list(f, g), minimal_list(1, '2', 3.3)) == minimal_list(f(1), f('2'), f(3.3), g(1), g('2'), g(3.3)));
}
