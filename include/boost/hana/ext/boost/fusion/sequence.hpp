/*!
@file
Defines `boost::hana::ext::boost::fusion::sequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_HPP

#include <boost/hana/fwd/ext/boost/fusion/sequence.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/forward.hpp>

// instances
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>

#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<ext::boost::fusion::sequence>
        : Iterable::mcd
    {
        template <typename Xs>
        static constexpr auto is_empty_impl(Xs&& xs) {
            using R = decltype(::boost::fusion::empty(detail::std::forward<Xs>(xs)));
            return bool_<R::value>;
        }

        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return ::boost::fusion::front(detail::std::forward<Xs>(xs));
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            return ::boost::fusion::pop_front(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct List::instance<ext::boost::fusion::sequence>
        : List::mcd<ext::boost::fusion::sequence>
    {
        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            return ::boost::fusion::push_front(
                    detail::std::forward<Xs>(xs),
                    detail::std::forward<X>(x));
        }

        static auto nil_impl()
        { return ::boost::fusion::nil{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_HPP
