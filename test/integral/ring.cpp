/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <test/laws/ring.hpp>
#include <type_traits>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    using C = std::common_type_t<T, U>;

    // one
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(one<Integral>, integral<T, 1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(one<Integral>, integral<U, 1>));
    }

    // mult
    {
        constexpr T x = 6;
        constexpr U y = 4;

        // Integral + Integral
        BOOST_HANA_CONSTANT_ASSERT(equal(
            mult(integral<T, x>, integral<U, y>),
            integral<C, x * y>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            mult(integral<T, x>, integral<U, y>),
            integral<C, x * y>
        ));

        // Integral * arithmetic
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mult(integral<T, x>, U{y}),
            C{x * y}
        ));

        // arithmetic * Integral
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mult(T{x}, integral<U, y>),
            C{x * y}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Ring_laws<Integral>(
            integral<T, 0>, integral<T, 1>, integral<U, 2>, integral<U, 3>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Ring_laws<Integral>(
            integral<T, 0>, integral<T, 1>, U{2}, U{3}
        ));
    }
}

int main() {
    tests<int, short>();
    tests<int, int>();
    tests<int, long>();
}
