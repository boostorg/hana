/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_MONOID_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_MONOID_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/monoid/mcd.hpp>
#include <test/numeric/numeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Monoid::instance<test::Numeric, test::Numeric> : Monoid::mcd {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y)
        { return test::numeric(x.value + y.value); }

        static BOOST_HANA_CONSTEXPR_LAMBDA auto zero_impl()
        { return test::numeric(0); }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_MONOID_HPP
