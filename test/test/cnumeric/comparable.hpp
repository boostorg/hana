/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_COMPARABLE_HPP

// Define either one to select which MCD is used:
//  BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD
//  BOOST_HANA_TEST_COMPARABLE_NOT_EQUAL_MCD
//
// If neither is defined, the MCD used is unspecified.


#if defined(BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD)

#include <boost/hana/comparable/equal_mcd.hpp>
#include <test/cnumeric/logical.hpp>
#include <test/cnumeric/cnumeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<test::CNumeric, test::CNumeric>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value == Y::value>; }
    };
}}

#else

#include <boost/hana/comparable/not_equal_mcd.hpp>
#include <test/cnumeric/logical.hpp>
#include <test/cnumeric/cnumeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<test::CNumeric, test::CNumeric>
        : Comparable::not_equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value != Y::value>; }
    };
}}

#endif

#endif //! BOOST_HANA_TEST_TEST_CNUMERIC_COMPARABLE_HPP
