/*!
 * @file
 * Defines `boost::hana::detail::left_folds::variadic`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_HPP
#define BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_HPP

#include <boost/hana/detail/left_folds/variadic_unrolled.hpp>


namespace boost { namespace hana { namespace detail { namespace left_folds {
    template <typename F, typename State, typename ...Xs>
    auto variadic(F f, State s, Xs ...xs)
    { return variadic_unrolled(f, s, xs...); }
}}}} // end namespace boost::hana::detail::left_folds

#endif // !BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_HPP
