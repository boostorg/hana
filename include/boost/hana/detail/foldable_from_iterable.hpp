/*!
 * @file
 * Defines `boost::hana::detail::foldable_from_iterable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_FOLDABLE_FROM_ITERABLE_HPP
#define BOOST_HANA_DETAIL_FOLDABLE_FROM_ITERABLE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup details
    struct foldable_from_iterable : defaults<Foldable> {
        template <typename F, typename State, typename Iterable>
        static constexpr auto foldl_helper(F f, State s, Iterable xs, Bool<false>)
        { return foldl(f, f(s, head(xs)), tail(xs)); }

        template <typename F, typename State, typename Iterable>
        static constexpr auto foldl_helper(F f, State s, Iterable xs, Bool<true>)
        { return s; }

        template <typename F, typename State, typename Iterable>
        static constexpr auto foldl_impl(F f, State s, Iterable xs)
        { return foldl_helper(f, s, xs, is_empty(xs)); }



        template <typename F, typename Iterable>
        static constexpr auto foldl1_impl(F f, Iterable xs)
        { return foldl(f, head(xs), tail(xs)); }



        template <typename F, typename Iterable>
        static constexpr auto foldr1_helper(F f, Iterable xs, Bool<false>)
        { return f(head(xs), foldr1(f, tail(xs))); }

        template <typename F, typename Iterable>
        static constexpr auto foldr1_helper(F f, Iterable xs, Bool<true>)
        { return head(xs); }

        template <typename F, typename Iterable>
        static constexpr auto foldr1_impl(F f, Iterable xs)
        { return foldr1_helper(f, xs, is_empty(tail(xs))); }



        template <typename F, typename State, typename Iterable>
        static constexpr auto foldr_helper(F f, State s, Iterable xs, Bool<false>)
        { return f(head(xs), foldr(f, s, tail(xs))); }

        template <typename F, typename State, typename Iterable>
        static constexpr auto foldr_helper(F f, State s, Iterable xs, Bool<true>)
        { return s; }

        template <typename F, typename State, typename Iterable>
        static constexpr auto foldr_impl(F f, State s, Iterable xs)
        { return foldr_helper(f, s, xs, is_empty(xs)); }
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_FOLDABLE_FROM_ITERABLE_HPP
