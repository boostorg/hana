/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_HPP

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
        template <typename T>
        struct CNumeric { };

        template <typename T, T v>
        struct cnumeric_type {
            static constexpr T value = v;
            struct hana { using datatype = CNumeric<T>; };

            //! @todo Find a way to make this explicit.
            constexpr operator bool() const { return value; }
        };

        template <typename T, T v>
        constexpr cnumeric_type<T, v> cnumeric{};
    }

    template <typename T, typename C>
    struct to_impl<test::CNumeric<T>, C,
        when<is_an<IntegralConstant, C>()>
    > {
        template <typename X>
        static constexpr decltype(auto) apply(X x) {
            constexpr auto v = value(x);
            return test::cnumeric<T, v>;
        }
    };

    template <typename T>
    struct Constant::instance<test::CNumeric<T>>
        : Constant::mcd
    {
        template <typename X>
        static constexpr auto value_impl(X const&) {
            return X::value;
        }
    };

    template <typename T>
    struct IntegralConstant::instance<test::CNumeric<T>>
        : IntegralConstant::mcd
    {
        template <T v>
        static constexpr auto integral_constant_impl() {
            return test::cnumeric<T, v>;
        }
    };

    template <typename T>
    struct Logical::instance<test::CNumeric<T>> : Logical::mcd {
        template <typename Then, typename Else>
        static constexpr auto
        eval_if_impl(decltype(test::cnumeric<bool, true>), Then t, Else e) {
            auto id = [](auto x) { return x; };
            return t(id);
        }

        template <typename Then, typename Else>
        static constexpr auto
        eval_if_impl(decltype(test::cnumeric<bool, false>), Then t, Else e) {
            auto id = [](auto x) { return x; };
            return e(id);
        }

        template <typename Cond, typename Then, typename Else>
        static constexpr auto eval_if_impl(Cond c, Then t, Else e) {
            return eval_if_impl(
                test::cnumeric<bool, static_cast<bool>(Cond::value)>, t, e
            );
        }

        template <typename X>
        static constexpr auto not_impl(X x)
        { return test::cnumeric<bool, !X::value>; }

        template <typename Pred, typename State, typename F>
        static constexpr auto
        while_helper(decltype(test::cnumeric<bool, false>), Pred pred, State state, F f)
        { return state; }

        template <typename Pred, typename State, typename F>
        static constexpr auto
        while_helper(decltype(test::cnumeric<bool, true>), Pred pred, State state, F f)
        { return while_(pred, f(state), f); }

        template <typename Pred, typename State, typename F>
        static constexpr auto
        while_impl(Pred pred, State state, F f) {
            return while_helper(
                test::cnumeric<bool, static_cast<bool>(value(pred(state)))>,
                pred, state, f
            );
        }
    };

    template <typename T, typename U>
    struct Orderable::instance<test::CNumeric<T>, test::CNumeric<U>>
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
    template <typename T, typename U>
    struct Comparable::instance<test::CNumeric<T>, test::CNumeric<U>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value == Y::value>; }
    };
#else
    template <typename T, typename U>
    struct Comparable::instance<test::CNumeric<T>, test::CNumeric<U>>
        : Comparable::not_equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value != Y::value>; }
    };
#endif
}}

#endif // !BOOST_HANA_TEST_TEST_CNUMERIC_HPP
