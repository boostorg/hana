/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_ORDERABLE_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_ORDERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> // for Orderable_laws with foreign types
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/orderable.hpp>

#include <test/laws/orderable.hpp>


template <typename I, typename T, typename U>
void IntegralConstant_orderable() {
    using namespace boost::hana;

    // less
    {
        // IntegralConstant < IntegralConstant
        BOOST_HANA_CONSTANT_ASSERT(less(
            integral_constant<I, T, 0>,
            integral_constant<I, U, 1>
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(less(
            integral_constant<I, T, 0>,
            integral_constant<I, U, 0>
        )));
        BOOST_HANA_CONSTANT_ASSERT(not_(less(
            integral_constant<I, T, 1>,
            integral_constant<I, U, 0>
        )));

        // IntegralConstant < other
        BOOST_HANA_CONSTEXPR_ASSERT(less(integral_constant<I, T, 0>, U{1}));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integral_constant<I, T, 0>, U{0})));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integral_constant<I, T, 1>, U{0})));

        // other < IntegralConstant
        BOOST_HANA_CONSTEXPR_ASSERT(less(T{0}, integral_constant<I, U, 1>));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(T{0}, integral_constant<I, U, 0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(T{1}, integral_constant<I, U, 0>)));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Orderable_laws(
            integral_constant<I, T, 0>, integral_constant<I, U, 0>,
            integral_constant<I, T, 1>, integral_constant<I, U, 1>,
            integral_constant<I, T, 2>, integral_constant<I, U, 2>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Orderable_laws(
            integral_constant<I, T, 0>,
            integral_constant<I, T, 1>,
            integral_constant<I, T, 2>,
            U{0}, U{1}, U{2}
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_ORDERABLE_HPP
