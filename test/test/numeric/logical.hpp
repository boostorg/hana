/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_LOGICAL_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_LOGICAL_HPP

#include <boost/hana/logical.hpp>
#include <test/numeric/numeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Logical::instance<test::Numeric> : Logical::mcd {
        template <typename C, typename T, typename E>
        static constexpr auto eval_if_impl(C c, T t, E e) {
            auto id = [](auto x) { return x; };
            return c.value ? t(id) : e(id);
        }

        template <typename X>
        static constexpr auto not_impl(X x)
        { return test::numeric(!x.value); }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_LOGICAL_HPP
