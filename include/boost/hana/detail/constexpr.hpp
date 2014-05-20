/*!
 * @file
 * Defines the `BOOST_HANA_CONSTEXPR_LAMBDA` macro.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CONSTEXPR_HPP
#define BOOST_HANA_DETAIL_CONSTEXPR_HPP

/*!
 * @ingroup details
 * Expands to `constexpr` if constexpr lambdas are supported and
 * to `const` otherwise.
 */
#define BOOST_HANA_CONSTEXPR_LAMBDA const

#endif // !BOOST_HANA_DETAIL_CONSTEXPR_HPP