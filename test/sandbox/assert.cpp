/*!
@file
Defines the `BOOST_HANA_STATIC_ASSERT` macro.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STATIC_ASSERT_HPP
#define BOOST_HANA_DETAIL_STATIC_ASSERT_HPP

#include <boost/hana/bool.hpp>

#include <cassert>


namespace boost { namespace hana { namespace detail {
    constexpr struct _assert {
        template <typename Expr>
        constexpr void operator()(Expr expr) const
        { call_impl(expr, expr); }

        template <typename Expr>
        static void call_impl(Expr expr, decltype(true_))
        { }

        template <typename Expr>
        static void call_impl(Expr expr, decltype(false_))
        { static_assert(expr, ""); }

        template <typename Expr>
        static void call_impl(Expr expr, bool, ...)
        { assert(expr); }
    } assert_{};

    //! @ingroup group-details
    //! Expands to a runtime assertion.
    //!
    //! Use this only at function scope. This is a workaround because
    //! constexpr lambdas are not available yet and so our unit tests
    //! can't use static_assert.
    #define BOOST_HANA_STATIC_ASSERT(...) BOOST_HANA_ASSERT(__VA_ARGS__)

    //! @ingroup group-details
    //! Performs either a runtime or a compile-time assertion depending on
    //! the type of the expression.
    #define BOOST_HANA_ASSERT(...) \
        ::boost::hana::detail::assert_((__VA_ARGS__))
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_STATIC_ASSERT_HPP



//////////////////////////////////////////////////////////////////////////////

int main() { }
