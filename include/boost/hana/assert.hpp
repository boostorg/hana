/*!
@file
Defines macros to perform different kinds of assertions.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ASSERT_HPP
#define BOOST_HANA_ASSERT_HPP

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/value.hpp>

#include <cstdio>
#include <cstdlib>


#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! @ingroup group-assertions
    //! Expands to a runtime assertion.
    //!
    //! This macro may only be used at function scope. Its argument must be
    //! explicitly convertible to a boolean and it must not be a `Constant`.
    //! If the expression is a `Constant`, a static assertion is triggered;
    //! use `BOOST_HANA_CONSTANT_ASSERT` instead.
#   define BOOST_HANA_RUNTIME_ASSERT(...) unspecified

    //! @ingroup group-assertions
    //! Compile-time assertion for `Constant`s.
    //!
    //! This macro may be used at global or function scope. Its argument must
    //! be a `Constant`, or else a compile-time assertion is triggered. If the
    //! argument is a `Constant`, its `value` is retrieved, it is converted to
    //! a boolean and `static_assert`ed upon. This kind of assertion is even
    //! stronger than a `static_assert`, since a `Constant` can yield a
    //! constant expression in any context.
    //!
    //! When the expression is a Constant, `BOOST_HANA_CONSTANT_ASSERT(expr)`
    //! is essentially equivalent to
    //! @code
    //!     auto temporary = expr;
    //!     static_assert(value(expr), "...");
    //! @endcode
    //!
    //! To understand why this is sometimes needed, especially when the
    //! expression contains a lambda, see the advanced section on
    //! [constexpr and side-effects](@ref tutorial-appendix-constexpr-effects).
#   define BOOST_HANA_CONSTANT_ASSERT(...) unspecified

    //! @ingroup group-assertions
    //! Expands to a static assertion or a runtime assertion, depending on
    //! whether `constexpr` lambdas are supported.
    //!
    //! This macro is used to assert on a condition that would be a constant
    //! expression if constexpr lambdas were supported.
    //!
    //! This macro may only be used at function scope. Its argument must be
    //! explicitly convertible to a boolean and it must not be a `Constant`.
    //! If the expression is a `Constant`, a static assertion is triggered;
    //! use `BOOST_HANA_CONSTANT_ASSERT` instead.
#   define BOOST_HANA_CONSTEXPR_ASSERT(...) unspecified

    //! @ingroup group-assertions
    //! Expands to an assertion that might be static or runtime, depending
    //! on whether the expression is a `Constant`.
    //!
    //! Specifically, if the expression is a `Constant`, this is equivalent
    //! to `BOOST_HANA_CONSTANT_ASSERT`. Otherwise, this is equivalent to
    //! `BOOST_HANA_RUNTIME_ASSERT`.
#   define BOOST_HANA_ASSERT(...) unspecified

    //! @ingroup group-assertions
    //! Equivalent to `BOOST_HANA_ASSERT`, to be used in unit tests.
    //!
    //! This macro is equivalent to `BOOST_HANA_ASSERT`, but the assertion is
    //! always performed, even when `BOOST_HANA_CONFIG_DISABLE_ASSERTIONS` is
    //! defined.
#   define BOOST_HANA_CHECK(...) unspecified

    //! @ingroup group-assertions
    //! Equivalent to `BOOST_HANA_CONSTEXPR_ASSERT`, to be used in unit tests.
    //!
    //! This macro is equivalent to `BOOST_HANA_CONSTEXPR_ASSERT`,
    //! but the assertion is always performed, even when
    //! `BOOST_HANA_CONFIG_DISABLE_ASSERTIONS` is defined.
#   define BOOST_HANA_CONSTEXPR_CHECK(...) unspecified

    //! @ingroup group-assertions
    //! Equivalent to `BOOST_HANA_CONSTANT_ASSERT`, to be used in unit tests.
    //!
    //! This macro is equivalent to `BOOST_HANA_CONSTANT_ASSERT`,
    //! but the assertion is always performed, even when
    //! `BOOST_HANA_CONFIG_DISABLE_ASSERTIONS` is defined.
#   define BOOST_HANA_CONSTANT_CHECK(...) unspecified

    //! @ingroup group-assertions
    //! Equivalent to `BOOST_HANA_RUNTIME_ASSERT`, to be used in unit tests.
    //!
    //! This macro is equivalent to `BOOST_HANA_RUNTIME_ASSERT`,
    //! but the assertion is always performed, even when
    //! `BOOST_HANA_CONFIG_DISABLE_ASSERTIONS` is defined.
#   define BOOST_HANA_RUNTIME_CHECK(...) unspecified

#else

#   define BOOST_HANA_PP_CAT_IMPL(x, y) x ## y
#   define BOOST_HANA_PP_CAT(x, y) BOOST_HANA_PP_CAT_IMPL(x, y)

#   define BOOST_HANA_RUNTIME_CHECK_IMPL(assert_or_check, tmpvar, expr)     \
        do {                                                                \
            auto tmpvar = expr;                                             \
            static_assert(!::boost::hana::Constant<decltype(tmpvar)>::value,\
            "the expression (" # expr ") yields a Constant; "               \
            "use BOOST_HANA_CONSTANT_" # assert_or_check " instead");       \
                                                                            \
            if (!static_cast<bool>(tmpvar)) {                               \
                ::std::fprintf(stderr, "Assertion failed: "                 \
                    "(%s), function %s, file %s, line %i.\n",               \
                    # expr, __func__, __FILE__, __LINE__);                  \
                ::std::abort();                                             \
            }                                                               \
        } while (false)                                                     \
    /**/

#   define BOOST_HANA_RUNTIME_CHECK(...)                                    \
        BOOST_HANA_RUNTIME_CHECK_IMPL(                                      \
            CHECK,                                                          \
            BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),                   \
            (__VA_ARGS__)                                                   \
        )                                                                   \
    /**/


#   define BOOST_HANA_CONSTANT_CHECK_IMPL(tmpvar, expr)                     \
        auto tmpvar = expr;                                                 \
        static_assert(::boost::hana::Constant<decltype(tmpvar)>::value,     \
        "the expression " # expr " does not yield a Constant");             \
                                                                            \
        static_assert(::boost::hana::value(tmpvar), # expr)                 \
    /**/

#   define BOOST_HANA_CONSTANT_CHECK(...)                                   \
        BOOST_HANA_CONSTANT_CHECK_IMPL(                                     \
            BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),                   \
            (__VA_ARGS__)                                                   \
        )                                                                   \
    /**/


#   define BOOST_HANA_CHECK_IMPL(tmpvar, EXPR)                              \
        do {                                                                \
            auto tmpvar = EXPR;                                             \
            ::boost::hana::if_(::boost::hana::bool_c<                       \
                ::boost::hana::Constant<decltype(tmpvar)>::value>,          \
                [](auto expr) {                                             \
                    static_assert(::boost::hana::value<decltype(expr)>(),   \
                    # EXPR);                                                \
                },                                                          \
                [](auto expr) {                                             \
                    if (!static_cast<bool>(expr)) {                         \
                        ::std::fprintf(stderr, "Assertion failed: "         \
                            "(%s), function %s, file %s, line %i.\n",       \
                            # EXPR, __func__, __FILE__, __LINE__);          \
                        ::std::abort();                                     \
                    }                                                       \
                }                                                           \
            )(tmpvar);                                                      \
        } while (false)                                                     \
    /**/

#   define BOOST_HANA_CHECK(...)                                            \
        BOOST_HANA_CHECK_IMPL(                                              \
            BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),                   \
            (__VA_ARGS__)                                                   \
        )                                                                   \
    /**/

#   if defined(BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA)
#       define BOOST_HANA_CONSTEXPR_CHECK_IMPL(assert_or_check, tmpvar, expr)\
            constexpr auto tmpvar = expr;                                   \
            static_assert(!::boost::hana::Constant<decltype(tmpvar)>::value,\
            "the expression " # expr " yields a Constant; "                 \
            "use BOOST_HANA_CONSTANT_" #assert_or_check " instead");        \
                                                                            \
            static_assert(static_cast<bool>(tmpvar), # expr)                \
        /**/

#       define BOOST_HANA_CONSTEXPR_CHECK(...)                              \
            BOOST_HANA_CONSTEXPR_CHECK_IMPL(                                \
                CHECK,                                                      \
                BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),               \
                (__VA_ARGS__)                                               \
            )                                                               \
        /**/
#   else
#       define BOOST_HANA_CONSTEXPR_CHECK_IMPL(_, __, expr) \
            BOOST_HANA_RUNTIME_CHECK(expr)

#       define BOOST_HANA_CONSTEXPR_CHECK(...) \
            BOOST_HANA_RUNTIME_CHECK(__VA_ARGS__)
#   endif // BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA


#   if defined(BOOST_HANA_CONFIG_DISABLE_ASSERTIONS)
#       define BOOST_HANA_CONSTANT_ASSERT(...)  /* nothing */
#       define BOOST_HANA_RUNTIME_ASSERT(...)   /* nothing */
#       define BOOST_HANA_CONSTEXPR_ASSERT(...) /* nothing */
#       define BOOST_HANA_ASSERT(...)           /* nothing */
#   else
#       define BOOST_HANA_CONSTANT_ASSERT(...) \
            BOOST_HANA_CONSTANT_CHECK(__VA_ARGS__)

#       define BOOST_HANA_RUNTIME_ASSERT(...)                               \
            BOOST_HANA_RUNTIME_CHECK_IMPL(                                  \
                ASSERT,                                                     \
                BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),               \
                (__VA_ARGS__)                                               \
            )                                                               \
        /**/

#       define BOOST_HANA_CONSTEXPR_ASSERT(...)                             \
            BOOST_HANA_CONSTEXPR_CHECK_IMPL(                                \
                ASSERT,                                                     \
                BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),               \
                (__VA_ARGS__)                                               \
            )                                                               \
        /**/

#       define BOOST_HANA_ASSERT(...) \
            BOOST_HANA_CHECK(__VA_ARGS__)
#   endif // BOOST_HANA_CONFIG_DISABLE_ASSERTIONS
#endif // BOOST_HANA_DOXYGEN_INVOKED

#endif // !BOOST_HANA_ASSERT_HPP
