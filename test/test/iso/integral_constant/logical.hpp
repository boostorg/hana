/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_LOGICAL_HPP
#define BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_LOGICAL_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral_constant.hpp>

#include <test/injection.hpp>


template <typename I, typename T, typename U>
void IntegralConstant_logical() {
    using namespace boost::hana;

    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return x<0>; };
    BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return x<1>; };

    // eval_if
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            eval_if(integral_constant<I, bool, true>, t, e), x<0>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            eval_if(integral_constant<I, bool, false>, t, e), x<1>
        ));
    }

    // not_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            not_(integral_constant<I, bool, true>),
            integral_constant<I, bool, false>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            not_(integral_constant<I, bool, false>),
            not_(not_(integral_constant<I, bool, true>))
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_INTEGRAL_CONSTANT_LOGICAL_HPP
