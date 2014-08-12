/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <test/laws/integral_domain.hpp>
#include <type_traits>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    using C = std::common_type_t<T, U>;

    // quot
    {
        // Integral / Integral
        BOOST_HANA_CONSTANT_ASSERT(equal(
            quot(integral<T, 6>, integral<U, 3>),
            integral<C, 6 / 3>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            quot(integral<T, 4>, integral<U, 3>),
            integral<C, 4 / 3>
        ));

        // Integral / arithmetic
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(integral<T, 6>, U{3}),
            C{6 / 3}
        ));

        // arithmetic / Integral
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(T{6}, integral<U, 3>),
            C{6 / 3}
        ));
    }

    // mod
    {
        // Integral % Integral
        BOOST_HANA_CONSTANT_ASSERT(equal(
            mod(integral<T, 6>, integral<U, 4>),
            integral<C, 6 % 4>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            mod(integral<T, 6>, integral<U, 4>),
            integral<C, 6 % 4>
        ));

        // Integral % arithmetic
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(integral<T, 6>, U{4}),
            C{6 % 4}
        ));

        // arithmetic % Integral
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(T{6}, integral<U, 4>),
            C{6 % 4}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(IntegralDomain_laws<Integral>(
            integral<T, 0>, integral<T, 1>, integral<U, 2>, integral<U, 3>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(IntegralDomain_laws<Integral>(
            integral<T, 0>, integral<T, 1>, U{2}, U{3}
        ));
    }
}

int main() {
    tests<int, short>();
    tests<int, int>();
    tests<int, long>();
}
