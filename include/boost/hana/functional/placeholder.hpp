/*!
@file
Defines `boost::hana::_`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_PLACEHOLDER_HPP
#define BOOST_HANA_FUNCTIONAL_PLACEHOLDER_HPP

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Create simple functions representing C++ operators inline.
    //!
    //! Specifically, `_` is an object used as a placeholder to build
    //! function objects representing calls to C++ operators. It works
    //! by overloading the operators between `_` and any object so that
    //! they return a function object which actually calls the corresponding
    //! operator on its argument(s). Hence, for any supported operator `@`:
    //! @code
    //!     (_ @ _)(x, y) == x @ y
    //! @endcode
    //!
    //! Operators may also be partially applied to one argument inline:
    //! @code
    //!     (x @ _)(y) == x @ y
    //!     (_ @ y)(x) == x @ y
    //! @endcode
    //!
    //! When invoked with more arguments than required, functions created with
    //! `_` will discard the superfluous instead of triggering an error:
    //! @code
    //!     (_ @ _)(x, y, z...) == x @ y
    //! @endcode
    //!
    //! This makes functions created with `_` easier to use in higher-order
    //! algorithms, which sometime provide more information than necessary
    //! to their callbacks.
    //!
    //! ### Supported operators
    //! - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    //! - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    //! - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    //! - %Logical: `||`, `&&`, `!`
    //! - Member access: `*` (dereference), `[]` (array subscript)
    //! - Other: `()` (function call)
    //!
    //! More complex functionality like the ability to compose placeholders
    //! into larger function objects inline are not supported. This is on
    //! purpose; you should either use C++14 generic lambdas or a library
    //! like [Boost.Phoenix][] if you need bigger guns. The goal here is
    //! to save you a couple of characters in simple situations.
    //!
    //! ### Example
    //! @snippet example/functional/placeholder.cpp main
    //!
    //! [Boost.Phoenix]: http://www.boost.org/doc/libs/release/libs/phoenix/doc/html/index.html
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr unspecified _{};
#else
    namespace placeholder_detail {
        template <typename I>
        struct subscript {
            I i;

            template <typename Xs, typename ...Z>
            constexpr auto operator()(Xs&& xs, Z const& ...) const&
                -> decltype(detail::std::forward<Xs>(xs)[i])
            { return detail::std::forward<Xs>(xs)[i]; }

            template <typename Xs, typename ...Z>
            constexpr auto operator()(Xs&& xs, Z const& ...) &
                -> decltype(detail::std::forward<Xs>(xs)[i])
            { return detail::std::forward<Xs>(xs)[i]; }

            template <typename Xs, typename ...Z>
            constexpr auto operator()(Xs&& xs, Z const& ...) &&
                -> decltype(detail::std::forward<Xs>(xs)[detail::std::declval<I>()])
            { return detail::std::forward<Xs>(xs)[detail::std::move(i)]; }
        };

        template <typename X>
        struct invoke;

        template <typename ...X>
        struct invoke<detail::closure_impl<X...>> {
            detail::closure_impl<X...> x;

            template <typename F, typename ...Z>
            constexpr auto operator()(F&& f, Z const& ...) const&
                -> decltype(detail::std::forward<F>(f)(static_cast<X const&>(
                    detail::std::declval<detail::closure_impl<X...>>()
                ).get...))
            { return detail::std::forward<F>(f)(static_cast<X const&>(x).get...); }

            template <typename F, typename ...Z>
            constexpr auto operator()(F&& f, Z const& ...) &
                -> decltype(detail::std::forward<F>(f)(static_cast<X&>(
                    detail::std::declval<detail::closure_impl<X...>&>()
                ).get...))
            { return detail::std::forward<F>(f)(static_cast<X&>(x).get...); }

            template <typename F, typename ...Z>
            constexpr auto operator()(F&& f, Z const& ...) &&
                -> decltype(detail::std::forward<F>(f)(static_cast<X&&>(
                    detail::std::declval<detail::closure_impl<X...>>()
                ).get...))
            { return detail::std::forward<F>(f)(static_cast<X&&>(x).get...); }
        };

        struct placeholder {
            template <typename X>
            constexpr decltype(auto) operator[](X&& x) const
            { return detail::create<subscript>{}(detail::std::forward<X>(x)); }

            template <typename ...X>
            constexpr decltype(auto) operator()(X&& ...x) const {
                return detail::create<invoke>{}(
                    detail::create<detail::closure>{}(
                        detail::std::forward<X>(x)...
                    )
                );
            }
        };

#define BOOST_HANA_PLACEHOLDER_BINARY_OP(op, op_name)                           \
    template <typename X>                                                       \
    struct op_name ## _left {                                                   \
        X x;                                                                    \
                                                                                \
        template <typename Y, typename ...Z>                                    \
        constexpr auto operator()(Y&& y, Z const& ...) const& -> decltype(      \
            detail::std::declval<X const&>() op detail::std::forward<Y>(y))     \
        { return x op detail::std::forward<Y>(y); }                             \
                                                                                \
        template <typename Y, typename ...Z>                                    \
        constexpr auto operator()(Y&& y, Z const& ...) & -> decltype(           \
            detail::std::declval<X&>() op detail::std::forward<Y>(y))           \
        { return x op detail::std::forward<Y>(y); }                             \
                                                                                \
        template <typename Y, typename ...Z>                                    \
        constexpr auto operator()(Y&& y, Z const& ...) && -> decltype(          \
            detail::std::declval<X>() op detail::std::forward<Y>(y))            \
        { return detail::std::move(x) op detail::std::forward<Y>(y); }          \
    };                                                                          \
                                                                                \
    template <typename Y>                                                       \
    struct op_name ## _right {                                                  \
        Y y;                                                                    \
                                                                                \
        template <typename X, typename ...Z>                                    \
        constexpr auto operator()(X&& x, Z const& ...) const& -> decltype(      \
            detail::std::forward<X>(x) op detail::std::declval<Y const&>())     \
        { return detail::std::forward<X>(x) op y; }                             \
                                                                                \
        template <typename X, typename ...Z>                                    \
        constexpr auto operator()(X&& x, Z const& ...) & -> decltype(           \
            detail::std::forward<X>(x) op detail::std::declval<Y&>())           \
        { return detail::std::forward<X>(x) op y; }                             \
                                                                                \
        template <typename X, typename ...Z>                                    \
        constexpr auto operator()(X&& x, Z const& ...) && -> decltype(          \
            detail::std::forward<X>(x) op detail::std::declval<Y>())            \
        { return detail::std::forward<X>(x) op detail::std::move(y); }          \
    };                                                                          \
                                                                                \
    struct op_name {                                                            \
        template <typename X, typename Y, typename ...Z>                        \
        constexpr auto operator()(X&& x, Y&& y, Z const& ...) const -> decltype(\
            detail::std::forward<X>(x) op detail::std::forward<Y>(y))           \
        { return detail::std::forward<X>(x) op detail::std::forward<Y>(y); }    \
    };                                                                          \
                                                                                \
    template <typename X>                                                       \
    constexpr decltype(auto) operator op (X&& x, placeholder)                   \
    { return detail::create<op_name ## _left>{}(detail::std::forward<X>(x)); }  \
                                                                                \
    template <typename Y>                                                       \
    constexpr decltype(auto) operator op (placeholder, Y&& y)                   \
    { return detail::create<op_name ## _right>{}(detail::std::forward<Y>(y)); } \
                                                                                \
    inline constexpr decltype(auto) operator op (placeholder, placeholder)      \
    { return op_name{}; }                                                       \
/**/

#define BOOST_HANA_PLACEHOLDER_UNARY_OP(op, op_name)                        \
    struct op_name {                                                        \
        template <typename X, typename ...Z>                                \
        constexpr auto operator()(X&& x, Z const& ...) const                \
            -> decltype(op detail::std::forward<X>(x))                      \
        { return op detail::std::forward<X>(x); }                           \
    };                                                                      \
                                                                            \
    inline constexpr decltype(auto) operator op (placeholder)               \
    { return op_name{}; }                                                   \
/**/
            // Arithmetic
            BOOST_HANA_PLACEHOLDER_UNARY_OP(+, unary_plus)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(-, unary_minus)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(+, plus)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(-, minus)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(*, times)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(/, divide)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(%, modulo)

            // Bitwise
            BOOST_HANA_PLACEHOLDER_UNARY_OP(~, bitwise_not)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(&, bitwise_and)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(|, bitwise_or)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(^, bitwise_xor)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(<<, left_shift)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(>>, right_shift)

            // Comparison
            BOOST_HANA_PLACEHOLDER_BINARY_OP(==, equal)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(!=, not_equal)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(<, less)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(<=, less_equal)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(>, greater)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(>=, greater_equal)

            // Logical
            BOOST_HANA_PLACEHOLDER_BINARY_OP(||, logical_or)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(&&, logical_and)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(!, logical_not)

            // Member access (array subscript is a member function)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(*, dereference)

            // Other (function call is a member function)

#undef BOOST_HANA_PREFIX_PLACEHOLDER_OP
#undef BOOST_HANA_BINARY_PLACEHOLDER_OP
    } // end namespace placeholder_detail

    constexpr placeholder_detail::placeholder _{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_PLACEHOLDER_HPP
