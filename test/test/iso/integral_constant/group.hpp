/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_GROUP_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_GROUP_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> // for Group_laws with foreign types
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>

#include <test/laws/group.hpp>
#include <type_traits>


template <typename I, typename T, typename U>
void IntegralConstant_group() {
    using namespace boost::hana;

    // negate
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            negate(integral_constant<I, T, 1>),
            integral_constant<I, T, -1>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            negate(integral_constant<I, U, 3>),
            integral_constant<I, U, -3>
        ));
    }

    // minus
    {
        using C = std::common_type_t<T, U>;

        // IntegralConstant - IntegralConstant
        BOOST_HANA_CONSTANT_ASSERT(equal(
            minus(integral_constant<I, T, 1>, integral_constant<I, U, 3>),
            integral_constant<I, C, 1 - 3>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            minus(integral_constant<I, T, 4>, integral_constant<I, U, 2>),
            integral_constant<I, C, 4 - 2>
        ));

        // IntegralConstant - other
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            minus(integral_constant<I, T, 1>, U{3}),
            C{1 - 3}
        ));

        // other - IntegralConstant
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            minus(T{1}, integral_constant<I, U, 3>),
            C{1 - 3}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Group_laws<I>(
            integral_constant<I, T, 0>, integral_constant<I, T, 1>, integral_constant<I, T, 2>,
            integral_constant<I, U, 0>, integral_constant<I, U, 1>, integral_constant<I, U, 2>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Group_laws<I>(
            integral_constant<I, T, 0>, integral_constant<I, T, 1>, integral_constant<I, T, 2>,
            U{0}, U{1}, U{3}
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_GROUP_HPP
