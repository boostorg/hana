/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_COMPARABLE_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> // for Comparable_laws with foreign types
#include <boost/hana/integral_constant/integral_constant.hpp>

#include <test/laws/comparable.hpp>


template <typename I, typename T, typename U>
void IntegralConstant_comparable() {
    using namespace boost::hana;

    // equal
    {
        // IntegralConstant == IntegralConstant
        BOOST_HANA_CONSTANT_ASSERT(equal(
            integral_constant<I, T, 0>,
            integral_constant<I, U, 0>
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            integral_constant<I, T, 0>,
            integral_constant<I, U, 1>
        )));


        // IntegralConstant == other
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            integral_constant<I, T, 0>, U{0}
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(
            integral_constant<I, T, 0>, U{1}
        )));


        // other == IntegralConstant
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            U{0}, integral_constant<I, T, 0>
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(
            U{1}, integral_constant<I, T, 0>
        )));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            integral_constant<I, T, 0>, integral_constant<I, U, 0>,
            integral_constant<I, T, 1>, integral_constant<I, U, 1>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Comparable_laws(
            integral_constant<I, T, 0>, integral_constant<I, U, 0>,
            integral_constant<I, T, 1>, integral_constant<I, U, 1>,
            T{0}, T{1}, U{0}, U{1}
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_COMPARABLE_HPP
