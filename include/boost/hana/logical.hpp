/*!
 * @file
 * Defines logical operations and branching.
 *
 *
 * @todo
 * - Use a non-naive implementation for `and_` and `or_`.
 * - How to short-circuit?
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_HPP

#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    constexpr struct _and {
        template <typename X, typename ...Xs>
        constexpr auto operator()(X x, Xs ...xs) const
        { return x && (*this)(xs...); }

        constexpr Bool<true> operator()() const
        { return {}; }
    } and_{};

    constexpr struct _or {
        template <typename X, typename ...Xs>
        constexpr auto operator()(X x, Xs ...xs) const
        { return x || (*this)(xs...); }

        constexpr Bool<false> operator()() const
        { return {}; }
    } or_{};

    constexpr struct _if {
        template <typename Then, typename Else>
        constexpr auto operator()(Bool<true>, Then t, Else) const
        { return t; }

        template <typename Then, typename Else>
        constexpr auto operator()(Bool<false>, Then, Else e) const
        { return e; }

        template <typename Then, typename Else>
        constexpr auto operator()(bool cond, Then t, Else e) const
        { return cond ? t : e; }
    } if_{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP
