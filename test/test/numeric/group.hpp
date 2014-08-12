/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_GROUP_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_GROUP_HPP

// Define either one to select which MCD is used:
//  BOOST_HANA_TEST_GROUP_NEGATE_MCD
//  BOOST_HANA_TEST_GROUP_MINUS_MCD
//
// If neither is defined, the MCD used is unspecified.


#if defined(BOOST_HANA_TEST_GROUP_NEGATE_MCD)

#include <test/numeric/numeric.hpp>
#include <boost/hana/group/negate_mcd.hpp>
#include <test/numeric/monoid.hpp>

namespace boost { namespace hana {
    template <>
    struct Group::instance<test::Numeric, test::Numeric>
        : Group::negate_mcd<test::Numeric, test::Numeric>
    {
        template <typename X>
        static constexpr auto negate_impl(X x)
        { return test::numeric(-x.value); }
    };
}}

#else

#include <test/numeric/numeric.hpp>
#include <boost/hana/group/minus_mcd.hpp>
#include <test/numeric/monoid.hpp>

namespace boost { namespace hana {
    template <>
    struct Group::instance<test::Numeric, test::Numeric>
        : Group::minus_mcd<test::Numeric, test::Numeric>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return test::numeric(x.value - y.value); }
    };
}}

#endif

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_GROUP_HPP
