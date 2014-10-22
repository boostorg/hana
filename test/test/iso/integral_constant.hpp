/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_HPP

#include <test/iso/integral_constant/comparable.hpp>
#include <test/iso/integral_constant/enumerable.hpp>
#include <test/iso/integral_constant/group.hpp>
#include <test/iso/integral_constant/integral_domain.hpp>
#include <test/iso/integral_constant/logical.hpp>
#include <test/iso/integral_constant/monoid.hpp>
#include <test/iso/integral_constant/orderable.hpp>
#include <test/iso/integral_constant/ring.hpp>


template <typename I, typename T, typename U>
void IntegralConstant_isomorphism() {
    IntegralConstant_comparable<I, T, U>();
    IntegralConstant_orderable<I, T, U>();

    IntegralConstant_enumerable<I, T>();
    IntegralConstant_logical<I, T, U>();

    IntegralConstant_monoid<I, T, U>();
    IntegralConstant_group<I, T, U>();
    IntegralConstant_ring<I, T, U>();
    IntegralConstant_integral_domain<I, T, U>();
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_HPP
