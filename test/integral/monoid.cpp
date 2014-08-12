/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <test/laws/monoid.hpp>
#include <type_traits>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    using C = std::common_type_t<T, U>;

    // zero
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(zero<Integral>, integral<T, 0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(zero<Integral>, integral<U, 0>));
    }

    // plus
    {
        constexpr T x = 6;
        constexpr U y = 4;

        // Integral + Integral
        BOOST_HANA_CONSTANT_ASSERT(equal(
            plus(integral<T, x>, integral<U, y>),
            integral<C, x + y>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            plus(integral<T, x>, integral<U, y>),
            integral<C, x + y>
        ));

        // Integral + arithmetic
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            plus(integral<T, x>, U{y}),
            C{x + y}
        ));

        // arithmetic + Integral
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            plus(T{x}, integral<U, y>),
            C{x + y}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monoid_laws<Integral>(
            integral<T, 0>, integral<T, 1>, integral<U, 2>, integral<U, 3>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Monoid_laws<Integral>(
            integral<T, 0>, integral<T, 1>, U{2}, U{3}
        ));
    }
}

int main() {
    tests<int, short>();
    tests<int, int>();
    tests<int, long>();
}
