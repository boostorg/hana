/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_INTEGRAL_DOMAIN_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> // for IntegralDomain_laws with foreign types
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/integral_domain.hpp>

#include <test/laws/integral_domain.hpp>
#include <type_traits>


template <typename I, typename T, typename U>
void IntegralConstant_integral_domain() {
    using namespace boost::hana;

    using C = std::common_type_t<T, U>;

    // quot
    {
        // IntegralConstant / IntegralConstant
        BOOST_HANA_CONSTANT_ASSERT(equal(
            quot(integral_constant<I, T, 6>, integral_constant<I, U, 3>),
            integral_constant<I, C, 6 / 3>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            quot(integral_constant<I, T, 4>, integral_constant<I, U, 3>),
            integral_constant<I, C, 4 / 3>
        ));

        // IntegralConstant / other
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(integral_constant<I, T, 6>, U{3}),
            C{6 / 3}
        ));

        // other / IntegralConstant
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(T{6}, integral_constant<I, U, 3>),
            C{6 / 3}
        ));
    }

    // mod
    {
        // IntegralConstant % IntegralConstant
        BOOST_HANA_CONSTANT_ASSERT(equal(
            mod(integral_constant<I, T, 6>, integral_constant<I, U, 4>),
            integral_constant<I, C, 6 % 4>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            mod(integral_constant<I, T, 6>, integral_constant<I, U, 4>),
            integral_constant<I, C, 6 % 4>
        ));

        // IntegralConstant % other
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(integral_constant<I, T, 6>, U{4}),
            C{6 % 4}
        ));

        // other % IntegralConstant
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(T{6}, integral_constant<I, U, 4>),
            C{6 % 4}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(IntegralDomain_laws<I>(
            integral_constant<I, T, 0>, integral_constant<I, T, 1>,
            integral_constant<I, U, 2>, integral_constant<I, U, 3>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(IntegralDomain_laws<I>(
            integral_constant<I, T, 0>, integral_constant<I, T, 1>, U{2}, U{3}
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_INTEGRAL_DOMAIN_HPP
