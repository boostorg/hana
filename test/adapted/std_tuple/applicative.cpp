/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_tuple.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto tuple = [](auto ...xs) {
    return std::make_tuple(xs...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return std::make_tuple(1, xs...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto ...xs) {
    return std::make_tuple(2, xs...);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(unit<StdTuple>(int_<0>) == tuple(int_<0>));
    BOOST_HANA_STATIC_ASSERT(unit<StdTuple>(int_<1>) == tuple(int_<1>));
    BOOST_HANA_STATIC_ASSERT(unit<StdTuple>(3) == tuple(3));

    BOOST_HANA_STATIC_ASSERT(ap(tuple(), tuple()) == tuple());
    BOOST_HANA_STATIC_ASSERT(ap(tuple(), tuple(1)) == tuple());
    BOOST_HANA_STATIC_ASSERT(ap(tuple(), tuple(1, '2')) == tuple());
    BOOST_HANA_STATIC_ASSERT(ap(tuple(), tuple(1, '2', 3.3)) == tuple());

    BOOST_HANA_STATIC_ASSERT(ap(tuple(f), tuple()) == tuple());
    BOOST_HANA_STATIC_ASSERT(ap(tuple(f), tuple(1)) == tuple(f(1)));
    BOOST_HANA_STATIC_ASSERT(ap(tuple(f), tuple(1, '2')) == tuple(f(1), f('2')));
    BOOST_HANA_STATIC_ASSERT(ap(tuple(f), tuple(1, '2', 3.3)) == tuple(f(1), f('2'), f(3.3)));

    BOOST_HANA_STATIC_ASSERT(ap(tuple(f, g), tuple()) == tuple());
    BOOST_HANA_STATIC_ASSERT(ap(tuple(f, g), tuple(1)) == tuple(f(1), g(1)));
    BOOST_HANA_STATIC_ASSERT(ap(tuple(f, g), tuple(1, '2')) == tuple(f(1), f('2'), g(1), g('2')));
    BOOST_HANA_STATIC_ASSERT(ap(tuple(f, g), tuple(1, '2', 3.3)) == tuple(f(1), f('2'), f(3.3), g(1), g('2'), g(3.3)));
}
