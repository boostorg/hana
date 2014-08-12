/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_LOGICAL_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_LOGICAL_HPP

#include <boost/hana/logical/mcd.hpp>
#include <test/cnumeric/cnumeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Logical::instance<test::CNumeric> : Logical::mcd {
        template <typename T, typename E>
        static constexpr auto eval_if_impl(decltype(test::cnumeric<bool, true>), T t, E e) {
            auto id = [](auto x) { return x; };
            return t(id);
        }

        template <typename T, typename E>
        static constexpr auto eval_if_impl(decltype(test::cnumeric<bool, false>), T t, E e) {
            auto id = [](auto x) { return x; };
            return e(id);
        }

        template <typename C, typename T, typename E>
        static constexpr auto eval_if_impl(C c, T t, E e) {
            return eval_if_impl(test::cnumeric<bool, static_cast<bool>(C::value)>, t, e);
        }

        template <typename X>
        static constexpr auto not_impl(X x)
        { return test::cnumeric<bool, !X::value>; }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_CNUMERIC_LOGICAL_HPP
