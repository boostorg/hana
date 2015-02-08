/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    namespace test {
        struct numeric_type {
            int value;
            //! @todo Find a way to make this explicit.
            constexpr operator bool() const { return value; }
        };

        using Numeric = datatype_t<numeric_type>;

        struct _numeric {
            constexpr numeric_type operator()(int x) const {
                return {x};
            }
        };
        constexpr _numeric numeric{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD
    //  BOOST_HANA_TEST_COMPARABLE_NOT_EQUAL_MCD
    //  BOOST_HANA_TEST_COMPARABLE_ORDERABLE_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(test::Numeric)>
        : detail::std::true_type
    { };

#if defined(BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD)
    template <>
    struct equal_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value == y.value); }
    };
#elif defined(BOOST_HANA_TEST_COMPARABLE_NOT_EQUAL_MCD)
    template <>
    struct not_equal_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value != y.value); }
    };
#else
    template <>
    struct equal_impl<test::Numeric, test::Numeric>
        : Orderable::equal_impl<test::Numeric, test::Numeric>
    { };
#endif

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Orderable(test::Numeric)>
        : detail::std::true_type
    { };

    template <>
    struct less_impl<test::Numeric, test::Numeric> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::numeric(x.value < y.value); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Enumerable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Enumerable(test::Numeric)>
        : detail::std::true_type
    { };

    template <>
    struct succ_impl<test::Numeric> {
        template <typename N>
        static constexpr auto apply(N n)
        { return test::numeric(n.value + 1); }
    };

    template <>
    struct pred_impl<test::Numeric> {
        template <typename N>
        static constexpr auto apply(N n)
        { return test::numeric(n.value - 1); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Logical(test::Numeric)>
        : detail::std::true_type
    { };

    template <>
    struct eval_if_impl<test::Numeric> {
        template <typename C, typename T, typename E>
        static constexpr auto apply(C c, T t, E e) {
            auto id = [](auto x) { return x; };
            return c.value ? t(id) : e(id);
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
    struct models<Monoid(test::Numeric)>
        : detail::std::true_type
    { };

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
    template <>
    struct models<Group(test::Numeric)>
        : detail::std::true_type
    { };

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
    struct models<Ring(test::Numeric)>
        : detail::std::true_type
    { };

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
    struct models<IntegralDomain(test::Numeric)>
        : detail::std::true_type
    { };

    template <>
    struct quot_impl<test::Numeric, test::Numeric> {
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
