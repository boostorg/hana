/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_ENUMERABLE_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_ENUMERABLE_HPP

#include <boost/hana/enumerable/mcd.hpp>
#include <test/numeric/numeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Enumerable::instance<test::Numeric> : Enumerable::mcd {
        template <typename N>
        static constexpr auto succ_impl(N n) {
            return test::numeric(n.value + 1);
        }

        template <typename N>
        static constexpr auto pred_impl(N n) {
            return test::numeric(n.value - 1);
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_ENUMERABLE_HPP
