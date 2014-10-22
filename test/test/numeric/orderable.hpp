/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_ORDERABLE_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_ORDERABLE_HPP

#include <boost/hana/orderable.hpp>
#include <test/numeric/logical.hpp>
#include <test/numeric/numeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Orderable::instance<test::Numeric, test::Numeric>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y)
        { return test::numeric(x.value < y.value); }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_ORDERABLE_HPP
