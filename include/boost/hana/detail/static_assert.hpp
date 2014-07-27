/*!
@file
Defines the `BOOST_HANA_STATIC_ASSERT` macro.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STATIC_ASSERT_HPP
#define BOOST_HANA_DETAIL_STATIC_ASSERT_HPP

#ifdef BOOST_HANA_DOXYGEN_INVOKED

    //! @ingroup details
    //! Expands to a `static_assert` or to an `assert`, depending on
    //! whether constexpr lambdas are available.
    //!
    //! Use this only at function scope. This is a workaround because
    //! constexpr lambdas are not available yet and so our unit tests
    //! can't use `static_assert`.
#   define BOOST_HANA_STATIC_ASSERT(...) static_assert((__VA_ARGS__), "")

#else

#   include <cassert>
#   define BOOST_HANA_STATIC_ASSERT(...) assert((__VA_ARGS__))

#endif

#endif // !BOOST_HANA_DETAIL_STATIC_ASSERT_HPP
