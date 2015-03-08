/*!
@file
Defines configuration macros used throughout the library.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONFIG_HPP
#define BOOST_HANA_CONFIG_HPP

//! @internal
//! Transforms a (version, revision, patchlevel) triple into a number of the
//! form 0xVVRRPPPP to allow comparing versions in a normalized way.
//!
//! See http://sourceforge.net/p/predef/wiki/VersionNormalization.
#define BOOST_HANA_CONFIG_VERSION(version, revision, patch) \
    (((version) << 24) + ((revision) << 16) + (patch))


//////////////////////////////////////////////////////////////////////////////
// Detect the compiler (currently only Clang is supported)
//////////////////////////////////////////////////////////////////////////////
#if defined(__clang__) && !defined(__apple_build_version__)

#   define BOOST_HANA_CONFIG_CLANG BOOST_HANA_CONFIG_VERSION(               \
            __clang_major__, __clang_minor__, __clang_patchlevel__)

#   if BOOST_HANA_CONFIG_CLANG < BOOST_HANA_CONFIG_VERSION(3, 5, 2)
#       error Versions of Clang prior to 3.5.2 are not supported.
#   endif

#else

#   error Your compiler is not officially supported by Hana or it was \
          not detected properly.

#endif

//////////////////////////////////////////////////////////////////////////////
// Detect the standard library
//////////////////////////////////////////////////////////////////////////////

// We include this empty header, which gives the chance to the standard
// library to define its macros so we can detect it. See http://goo.gl/eXNYJH.
#include <ciso646>

#if defined(_LIBCPP_VERSION)

#   define BOOST_HANA_CONFIG_LIBCPP BOOST_HANA_CONFIG_VERSION(              \
                ((_LIBCPP_VERSION) / 1000) % 10, 0, (_LIBCPP_VERSION) % 1000)

#   if BOOST_HANA_CONFIG_LIBCPP < BOOST_HANA_CONFIG_VERSION(1, 0, 101)
#       error Versions of libc++ prior to the one shipped with Clang 3.5.2 \
              are not supported.
#   endif

#else

#   error Your standard library is not officially supported by Hana or it \
          was not detected properly.

#endif


//////////////////////////////////////////////////////////////////////////////
// Caveats and other compiler-dependent options
//////////////////////////////////////////////////////////////////////////////

// Enables some optimizations based on C++1z fold-expressions.
// Currently always disabled.
#if 0
#   define BOOST_HANA_CONFIG_HAS_CXX1Z_FOLD_EXPRESSIONS
#endif

// BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA enables some constructs requiring
// `constexpr` lambdas, which are not in the language (yet).
// Currently always disabled.
//
// BOOST_HANA_CONSTEXPR_LAMBDA expands to `constexpr` if constexpr lambdas
// are supported and to nothing otherwise.
#if 0
#   define BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA
#   define BOOST_HANA_CONSTEXPR_LAMBDA constexpr
#else
#   define BOOST_HANA_CONSTEXPR_LAMBDA /* nothing */
#endif

// The std::tuple adapter is broken on libc++ prior to the one shipped
// with Clang 3.7.0.
#if defined(BOOST_HANA_CONFIG_LIBCPP) &&                                    \
        BOOST_HANA_CONFIG_LIBCPP < BOOST_HANA_CONFIG_VERSION(1, 0, 101)
#   define BOOST_HANA_CONFIG_HAS_NO_STD_TUPLE_ADAPTER
#endif

// There's a bug in std::tuple_cat in libc++ right now.
// See http://llvm.org/bugs/show_bug.cgi?id=22806.
#if defined(BOOST_HANA_CONFIG_LIBCPP)
#   define BOOST_HANA_CONFIG_LIBCPP_HAS_BUG_22806
#endif


//////////////////////////////////////////////////////////////////////////////
// Compiler-independent options that can be tweaked by users
//////////////////////////////////////////////////////////////////////////////

#if defined(BOOST_HANA_DOXYGEN_INVOKED) || \
    (defined(NDEBUG) && !defined(BOOST_HANA_CONFIG_DISABLE_PRECONDITIONS))
    //! @ingroup group-config
    //! Disables the checking of preconditions in tag-dispatched methods.
    //!
    //! Some tag-dispatched methods have preconditions that can be checked
    //! reasonably easily. When this is the case, these preconditions are
    //! checked automatically when the method is used. When this macro is
    //! defined, the checks are not performed.
    //!
    //! This macro is defined automatically when `NDEBUG` is defined. It can
    //! also be defined by users before including this header or defined on
    //! the command line.
    //!
    //! @note
    //! This does not disable the `BOOST_HANA_*_ASSERT` macro & friends.
    //! See @ref BOOST_HANA_CONFIG_DISABLE_ASSERTIONS for this.
#   define BOOST_HANA_CONFIG_DISABLE_PRECONDITIONS
#endif

#if defined(BOOST_HANA_DOXYGEN_INVOKED) || \
    (defined(NDEBUG) && !defined(BOOST_HANA_CONFIG_DISABLE_ASSERTIONS))
    //! @ingroup group-config
    //! Disables the `BOOST_HANA_*_ASSERT` macro & friends.
    //!
    //! When this macro is defined, the `BOOST_HANA_*_ASSERT` macro & friends
    //! are disabled, i.e. they expand to nothing.
    //!
    //! This macro is defined automatically when `NDEBUG` is defined. It can
    //! also be defined by users before including this header or defined on
    //! the command line.
    //!
    //! @note
    //! This macro has the side effect of disabling the checking of most
    //! preconditions in tag-dispatched methods, because those preconditions
    //! usually use the `BOOST_HANA_*_ASSERT` macros to do their job. However,
    //! if one wants to disable the checking of preconditions specifically,
    //! the @ref BOOST_HANA_CONFIG_DISABLE_PRECONDITIONS macro should be used
    //! instead.
#   define BOOST_HANA_CONFIG_DISABLE_ASSERTIONS
#endif

#define BOOST_HANA_CONFIG_CHECK_DATA_TYPES

#endif // !BOOST_HANA_CONFIG_HPP
