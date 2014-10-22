/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_INTEGRAL_DOMAIN_HPP

#include <boost/hana/integral_domain.hpp>
#include <test/numeric/numeric.hpp>

// Mcd
#include <test/numeric/ring.hpp>


namespace boost { namespace hana {
    template <>
    struct IntegralDomain::instance<test::Numeric, test::Numeric>
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y)
        { return test::numeric(x.value / y.value); }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y)
        { return test::numeric(x.value % y.value); }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_INTEGRAL_DOMAIN_HPP
