/*!
 * @file
 * Defines the `BOOST_HANA_STATIC_ASSERT` macro.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STATIC_ASSERT_HPP
#define BOOST_HANA_DETAIL_STATIC_ASSERT_HPP

#include <cassert>


/*!
 * @ingroup details
 * Expands to a runtime assertion.
 *
 * Use this only at function scope. This is a workaround because constexpr
 * lambdas are not available yet and so our unit tests can't use
 * static_assert.
 */
#define BOOST_HANA_STATIC_ASSERT(...) assert(__VA_ARGS__)

#endif // !BOOST_HANA_DETAIL_STATIC_ASSERT_HPP
