/*!
@file
Defines macros to perform different kinds of assertions.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_ASSERT_HPP
#define BOOST_HANA_DETAIL_ASSERT_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/is_a.hpp>

#include <cstdio>
#include <cstdlib>


#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! @ingroup group-details
    //! Expands to a runtime assertion.
    //!
    //! This macro may only be used at function scope. Its argument must be
    //! explicitly convertible to a boolean and its data type must not be a
    //! `Constant`. If the expression is a `Constant`, a static assertion is
    //! triggered; use `BOOST_HANA_CONSTANT_ASSERT` instead.
    //!
    //! @note
    //! This macro is meant to be used in the unit tests only.
    //!
    //! @todo
    //! Create another set of macros for unit tests, and use these macros
    //! inside the library.
#   define BOOST_HANA_RUNTIME_ASSERT(...) unspecified

    //! @ingroup group-details
    //! Compile-time assertion for `Constant`s.
    //!
    //! This macro may be used at global or function scope. Its argument must
    //! be a `Constant`, or else a compile-time assertion is triggered. If the
    //! argument is a `Constant`, its `value` is retrieved, it is converted to
    //! a boolean and `static_assert`ed upon. Note that any side effects
    //! performed by the assertion are executed.
#   define BOOST_HANA_CONSTANT_ASSERT(...) unspecified

    //! @ingroup group-details
    //! Expands to a static assertion or a runtime assertion, depending on
    //! whether `constexpr` lambdas are supported.
    //!
    //! This macro is used to assert on a condition that would be a constant
    //! expression if constexpr lambdas were supported.
    //!
    //! This macro may only be used at function scope. Its argument must be
    //! explicitly convertible to a boolean and its data type must not be a
    //! `Constant`. If the expression is a `Constant`, a static assertion is
    //! triggered; use `BOOST_HANA_CONSTANT_ASSERT` instead.
#   define BOOST_HANA_CONSTEXPR_ASSERT(...) unspecified
#else

#   define BOOST_HANA_PP_CAT_IMPL(x, y) x ## y
#   define BOOST_HANA_PP_CAT(x, y) BOOST_HANA_PP_CAT_IMPL(x, y)

#   define BOOST_HANA_RUNTIME_ASSERT(...)                                   \
        do {                                                                \
            auto _boost_hana_tmp_var = (__VA_ARGS__);                       \
            static_assert(!::boost::hana::value(                            \
                ::boost::hana::is_a< ::boost::hana::Constant>(              \
                    _boost_hana_tmp_var                                     \
                )                                                           \
            ),                                                              \
            "the expression (" # __VA_ARGS__ ") yields a Constant; "        \
            "use BOOST_HANA_CONSTANT_ASSERT instead");                      \
                                                                            \
            if (!static_cast<bool>(_boost_hana_tmp_var)) {                  \
                ::std::printf("Assertion failed: "                          \
                    "(%s), function %s, file %s, line %i.\n",               \
                    # __VA_ARGS__, __func__, __FILE__, __LINE__);           \
                ::std::abort();                                             \
            }                                                               \
        } while (false)                                                     \
    /**/

#   define BOOST_HANA_CONSTANT_ASSERT_IMPL(tmpvar, expr)                    \
        auto tmpvar = expr;                                                 \
        static_assert(::boost::hana::value(                                 \
            ::boost::hana::is_a< ::boost::hana::Constant>(tmpvar)           \
        ),                                                                  \
        "the expression " # expr " does not yield a Constant");             \
                                                                            \
        static_assert(::boost::hana::value(tmpvar), # expr)                 \
    /**/

#   define BOOST_HANA_CONSTANT_ASSERT(...)                                  \
        BOOST_HANA_CONSTANT_ASSERT_IMPL(                                    \
            BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),                   \
            (__VA_ARGS__)                                                   \
        )                                                                   \
    /**/

#   if 0 // unfortunately, constexpr lambdas are not supported
#       define BOOST_HANA_CONSTEXPR_ASSERT_IMPL(tmpvar, expr)               \
            constexpr auto tmpvar = expr;                                   \
            static_assert(!::boost::hana::value(                            \
                ::boost::hana::is_a< ::boost::hana::Constant>(tmpvar)       \
            ),                                                              \
            "the expression " # expr " yields a Constant; "                 \
            "use BOOST_HANA_CONSTANT_ASSERT instead");                      \
                                                                            \
            static_assert(static_cast<bool>(tmpvar), # expr)                \
        /**/

#       define BOOST_HANA_CONSTEXPR_ASSERT(...)                             \
            BOOST_HANA_CONSTEXPR_ASSERT_IMPL(                               \
                BOOST_HANA_PP_CAT(boost_hana_tmp_, __LINE__),               \
                (__VA_ARGS__)                                               \
            )                                                               \
        /**/
#   else
#       define BOOST_HANA_CONSTEXPR_ASSERT(...) \
            BOOST_HANA_RUNTIME_ASSERT(__VA_ARGS__)
#   endif
#endif

#endif // !BOOST_HANA_DETAIL_ASSERT_HPP
