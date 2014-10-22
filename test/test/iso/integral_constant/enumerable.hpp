/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_ENUMERABLE_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_ENUMERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/integral_constant.hpp>


template <typename I, typename T>
void IntegralConstant_enumerable() {
    using namespace boost::hana;

    // succ
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            succ(integral_constant<I, T, 0>),
            integral_constant<I, T, 1>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            succ(integral_constant<I, T, 1>),
            integral_constant<I, T, 2>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            succ(integral_constant<I, T, 2>),
            integral_constant<I, T, 3>
        ));
    }

    // pred
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            pred(integral_constant<I, T, 1>),
            integral_constant<I, T, 0>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            pred(integral_constant<I, T, 2>),
            integral_constant<I, T, 1>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            pred(integral_constant<I, T, 3>),
            integral_constant<I, T, 2>
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_ENUMERABLE_HPP
