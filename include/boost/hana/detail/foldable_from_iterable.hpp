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
#include <boost/hana/functional.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup details
    struct foldable_from_iterable : defaults<Foldable> {
        template <typename F, typename State, typename Iterable>
        static constexpr auto foldl_impl(F f, State s, Iterable xs) {
            return if_(is_empty(xs),
                always(s),
                [=](auto xs) { return foldl(f, f(s, head(xs)), tail(xs)); }
            )(xs);
        }

        template <typename F, typename Iterable>
        static constexpr auto foldl1_impl(F f, Iterable xs)
        { return foldl(f, head(xs), tail(xs)); }

        template <typename F, typename Iterable>
        static constexpr auto foldr1_impl(F f, Iterable xs) {
            return if_(is_empty(tail(xs)),
                head,
                [=](auto xs) { return f(head(xs), foldr1(f, tail(xs))); }
            )(xs);
        }

        template <typename F, typename State, typename Iterable>
        static constexpr auto foldr_impl(F f, State s, Iterable xs) {
            return if_(is_empty(xs),
                always(s),
                [=](auto xs) { return f(head(xs), foldr(f, s, tail(xs))); }
            )(xs);
        }
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_FOLDABLE_FROM_ITERABLE_HPP
