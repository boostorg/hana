/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_MONOID_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_MONOID_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> // for Monoid_laws with foreign types
#include <boost/hana/integral_constant/integral_constant.hpp>
#include <boost/hana/monoid/monoid.hpp>

#include <test/laws/monoid.hpp>
#include <type_traits>


template <typename I, typename T, typename U>
void IntegralConstant_monoid() {
    using namespace boost::hana;

    using C = std::common_type_t<T, U>;

    // zero
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(zero<I>, integral_constant<I, T, 0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(zero<I>, integral_constant<I, U, 0>));
    }

    // plus
    {
        constexpr T x = 6;
        constexpr U y = 4;

        // IntegralConstant + IntegralConstant
        BOOST_HANA_CONSTANT_ASSERT(equal(
            plus(integral_constant<I, T, x>, integral_constant<I, U, y>),
            integral_constant<I, C, x + y>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            plus(integral_constant<I, T, x>, integral_constant<I, U, y>),
            integral_constant<I, C, x + y>
        ));

        // IntegralConstant + other
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            plus(integral_constant<I, T, x>, U{y}),
            C{x + y}
        ));

        // other + IntegralConstant
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            plus(T{x}, integral_constant<I, U, y>),
            C{x + y}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monoid_laws<I>(
            integral_constant<I, T, 0>, integral_constant<I, T, 1>,
            integral_constant<I, U, 2>, integral_constant<I, U, 3>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Monoid_laws<I>(
            integral_constant<I, T, 0>, integral_constant<I, T, 1>,
            U{2}, U{3}
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_MONOID_HPP
