/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_HPP

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
        struct CNumeric { };

        template <typename T, T v>
        struct cnumeric_type {
            static constexpr T value = v;
            struct hana { using datatype = CNumeric; };

            //! @todo Find a way to make this explicit.
            constexpr operator bool() const { return value; }
        };

        template <typename T, T v>
        constexpr cnumeric_type<T, v> cnumeric{};
    }

    template <>
    struct Constant::instance<test::CNumeric>
        : Constant::mcd
    {
        template <typename X>
        static constexpr auto value_impl(X const&) {
            return X::value;
        }
    };

    template <>
    struct IntegralConstant::instance<test::CNumeric>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl() {
            return test::cnumeric<T, v>;
        }
    };

    template <>
    struct Logical::instance<test::CNumeric> : Logical::mcd {
        template <typename T, typename E>
        static constexpr auto
        eval_if_impl(decltype(test::cnumeric<bool, true>), T t, E e) {
            auto id = [](auto x) { return x; };
            return t(id);
        }

        template <typename T, typename E>
        static constexpr auto
        eval_if_impl(decltype(test::cnumeric<bool, false>), T t, E e) {
            auto id = [](auto x) { return x; };
            return e(id);
        }

        template <typename C, typename T, typename E>
        static constexpr auto eval_if_impl(C c, T t, E e) {
            return eval_if_impl(
                test::cnumeric<bool, static_cast<bool>(C::value)>, t, e
            );
        }

        template <typename X>
        static constexpr auto not_impl(X x)
        { return test::cnumeric<bool, !X::value>; }
    };

    template <>
    struct Orderable::instance<test::CNumeric, test::CNumeric>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y) {
            return test::cnumeric<bool, (X::value < Y::value)>;
        }
    };

    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD
    //  BOOST_HANA_TEST_COMPARABLE_NOT_EQUAL_MCD
    //
    // If neither is defined, the MCD used is unspecified.
#ifdef BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD
    template <>
    struct Comparable::instance<test::CNumeric, test::CNumeric>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value == Y::value>; }
    };
#else
    template <>
    struct Comparable::instance<test::CNumeric, test::CNumeric>
        : Comparable::not_equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value != Y::value>; }
    };
#endif
}}

#endif // !BOOST_HANA_TEST_TEST_CNUMERIC_HPP
