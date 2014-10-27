/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_INTEGRAL_CONSTANT_HPP

#include <boost/hana/integral_constant.hpp>
#include <test/cnumeric/cnumeric.hpp>

// Mcd
#include <test/cnumeric/constant.hpp>


namespace boost { namespace hana {
    template <>
    struct IntegralConstant::instance<test::CNumeric>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl() {
            return test::cnumeric<T, v>;
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_CNUMERIC_INTEGRAL_CONSTANT_HPP
