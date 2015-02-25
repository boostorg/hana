/*!
@file
Defines configuration macros used throughout the library.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONFIG_HPP
#define BOOST_HANA_CONFIG_HPP

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

#if defined(BOOST_HANA_DOXYGEN_INVOKED) || 0 // currently always disabled
    //! @ingroup group-config
    //! Enables some optimizations based on C++1z fold-expressions.
#   define BOOST_HANA_CONFIG_HAS_CXX1Z_FOLD_EXPRESSIONS
#endif

#if defined(BOOST_HANA_DOXYGEN_INVOKED) || 0 // currently always disabled
    //! @ingroup group-config
    //! Enables some constructs requiring `constexpr` lambdas, which are not
    //! in the language (yet).
#   define BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA

#endif


#ifdef BOOST_HANA_CONFIG_HAS_CONSTEXPR_LAMBDA
    //! @ingroup group-details
    //! Expands to `constexpr` if constexpr lambdas are supported and
    //! to nothing otherwise.
#   define BOOST_HANA_CONSTEXPR_LAMBDA constexpr
#else
#   define BOOST_HANA_CONSTEXPR_LAMBDA /* nothing */
#endif

#endif // !BOOST_HANA_CONFIG_HPP
