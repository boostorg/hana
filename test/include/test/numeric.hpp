/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_HPP

#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/eval.hpp>

// instances
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/group.hpp>
#include <boost/hana/concept/integral_domain.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/monoid.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/concept/ring.hpp>


namespace boost { namespace hana {
    namespace test {
        struct numeric_type {
            constexpr explicit numeric_type(int v) : value(v) { }
            int value;
            constexpr operator int() const { return value; }
        };

        using Numeric = tag_of_t<numeric_type>;

        struct numeric_t {
            constexpr numeric_type operator()(int x) const {
                return numeric_type{x};
            }
        };
        constexpr numeric_t numeric{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value == y.value); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y) {
            // Workaround a _weird_ GCC bug:
            // error: parse error in template argument list
            //      bool cmp = (x.value < y.value);
            //                    ^
            int xv = x.value, yv = y.value;
            return test::numeric(xv < yv);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct eval_if_impl<test::Numeric> {
        template <typename C, typename T, typename E>
        static constexpr auto apply(C c, T t, E e) {
            return c.value ? hana::eval(t) : hana::eval(e);
        }
    };

    template <>
    struct not_impl<test::Numeric> {
        template <typename X>
        static constexpr auto apply(X x)
        { return test::numeric(!x.value); }
    };

    template <>
    struct while_impl<test::Numeric> {
        template <typename Pred, typename State, typename F>
        static constexpr auto apply(Pred pred, State state, F f)
            -> decltype(true ? f(state) : state)
        {
            if (pred(state))
                return while_(pred, f(state), f);
            else
                return state;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monoid
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct plus_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value + y.value); }
    };

    template <>
    struct zero_impl<test::Numeric> {
        static constexpr auto apply()
        { return test::numeric(0); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Group
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_GROUP_NEGATE_MCD
    //  BOOST_HANA_TEST_GROUP_MINUS_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
#if defined(BOOST_HANA_TEST_GROUP_NEGATE_MCD)
    template <>
    struct negate_impl<test::Numeric> {
        template <typename X>
        static constexpr auto apply(X x)
        { return test::numeric(-x.value); }
    };
#else
    template <>
    struct minus_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value - y.value); }
    };
#endif

    //////////////////////////////////////////////////////////////////////////
    // Ring
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct mult_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value * y.value); }
    };

    template <>
    struct one_impl<test::Numeric> {
        static constexpr auto apply()
        { return test::numeric(1); }
    };

    //////////////////////////////////////////////////////////////////////////
    // IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct div_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value / y.value); }
    };

    template <>
    struct mod_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value % y.value); }
    };
}} // end namespace boost::hana

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_HPP
