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

#include <cassert>


namespace boost { namespace hana { namespace assert_detail {
    // If the argument is not a Constant, we avoid triggering an error or
    // the second static_assert in BOOST_HANA_CONSTANT_ASSERT.
    template <typename T, bool = is_a<Constant, datatype_t<T>>>
    constexpr bool value_helper = true;

    //! @todo
    //! Consider changing the `value` method so it becomes a variable template
    //! parameterized on the type of an object.
    template <typename T>
    constexpr bool value_helper<T, true> = value(T{});
}}}

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! @ingroup group-details
    //! Expands to a runtime assertion.
    //!
    //! This macro may only be used at function scope. Its argument must be
    //! explicitly convertible to a boolean and its data type must not be a
    //! `Constant`. If the expression is a `Constant`, a static assertion is
    //! triggered; use `BOOST_HANA_CONSTANT_ASSERT` instead.
    //!
    //! Because this macro uses `decltype` on its argument, a lambda may not
    //! be used inside the expression.
#   define BOOST_HANA_RUNTIME_ASSERT(...) unspecified

    //! @ingroup group-details
    //! Compile-time assertion for `Constant`s.
    //!
    //! This macro may be used at any scope where a `static_assert` can be
    //! used. Its argument must be a `Constant`, or else a compile-time
    //! assertion is triggered. If the argument is a `Constant`, its `value`
    //! is retrieved, it is converted to a boolean and `static_assert`ed` upon.
    //!
    //! Because this macro uses `decltype` on its argument, a lambda may not
    //! be used inside the expression.
#   define BOOST_HANA_CONSTANT_ASSERT(...) unspecified

    //! @ingroup group-details
    //! Expands to a static assertion or a runtime assertion, depending on
    //! whether `constexpr` lambdas are supported.
    //!
    //! This macro is used to assert on a condition that would be a constant
    //! expression if constexpr lambdas were supported.
    //!
    //! This macro may only be used at function scope. Its argument must be
    //! convertible to a boolean and its data type must not be a `Constant`.
    //! If the expression is a `Constant`, a static assertion is triggered;
    //! use `BOOST_HANA_CONSTANT_ASSERT` instead.
    //!
    //! Because this macro uses `decltype` on its argument, a lambda may not
    //! be used inside the expression.
#   define BOOST_HANA_CONSTEXPR_ASSERT(...) unspecified
#else
#   define BOOST_HANA_RUNTIME_ASSERT(...)                                   \
        static_assert(!::boost::hana::value(                                \
            ::boost::hana::is_a<                                            \
                ::boost::hana::Constant,                                    \
                ::boost::hana::datatype_t<decltype(__VA_ARGS__)>            \
            >                                                               \
        ),                                                                  \
        "the expression (" # __VA_ARGS__ ") yields a Constant; "            \
        "use BOOST_HANA_CONSTANT_ASSERT instead");                          \
                                                                            \
        assert(static_cast<bool>(__VA_ARGS__))                              \
    /**/

#   define BOOST_HANA_CONSTANT_ASSERT(...)                                  \
        static_assert(::boost::hana::is_a<                                  \
            ::boost::hana::Constant,                                        \
            ::boost::hana::datatype_t<decltype(__VA_ARGS__)>                \
        >,                                                                  \
        "the expression (" # __VA_ARGS__ ") does not yield a Constant");    \
                                                                            \
        static_assert(::boost::hana::assert_detail::value_helper<           \
            decltype(__VA_ARGS__)                                           \
        >, # __VA_ARGS__)                                                   \
    /**/

#   if 0 // unfortunately, constexpr lambdas are not supported
#       define BOOST_HANA_CONSTEXPR_ASSERT(...)                             \
            static_assert(!::boost::hana::value(                            \
                ::boost::hana::is_a<                                        \
                    ::boost::hana::Constant,                                \
                    ::boost::hana::datatype_t<decltype(__VA_ARGS__)>        \
                >                                                           \
            ),                                                              \
            "the expression (" # __VA_ARGS__ ") yields a Constant; "        \
            "use BOOST_HANA_CONSTANT_ASSERT instead");                      \
                                                                            \
            static_assert((__VA_ARGS__), # __VA_ARGS__)                     \
        /**/
#   else
#       define BOOST_HANA_CONSTEXPR_ASSERT(...) \
            BOOST_HANA_RUNTIME_ASSERT(__VA_ARGS__)
#   endif
#endif

#endif // !BOOST_HANA_DETAIL_ASSERT_HPP
