/*!
@file
Defines `boost::hana::ext::boost::fusion::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP

#include <boost/hana/fwd/ext/boost/fusion/list.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>

// instances
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/searchable.hpp>

#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/container/generation/make_list.hpp>
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/container/list/convert.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(ext::boost::fusion::List)>
        : detail::std::true_type
    { };

    template <>
    struct foldl_impl<ext::boost::fusion::List>
        : Iterable::foldl_impl<ext::boost::fusion::List>
    { };

    template <>
    struct foldr_impl<ext::boost::fusion::List>
        : Iterable::foldr_impl<ext::boost::fusion::List>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Searchable(ext::boost::fusion::List)>
        : detail::std::true_type
    { };

    template <>
    struct find_impl<ext::boost::fusion::List>
        : Iterable::find_impl<ext::boost::fusion::List>
    { };

    template <>
    struct any_impl<ext::boost::fusion::List>
        : Iterable::any_impl<ext::boost::fusion::List>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Iterable(ext::boost::fusion::List)>
        : detail::std::true_type
    { };

    template <>
    struct head_impl<ext::boost::fusion::List> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::boost::fusion::front(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct tail_impl<ext::boost::fusion::List> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::boost::fusion::as_list(
                ::boost::fusion::pop_front(detail::std::forward<Xs>(xs)));
        }
    };

    template <>
    struct is_empty_impl<ext::boost::fusion::List> {
        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            using R = decltype(::boost::fusion::empty(detail::std::forward<Xs>(xs)));
            return bool_<R::value>;
        }
    };

    template <>
    struct List::instance<ext::boost::fusion::List>
        : List::mcd<ext::boost::fusion::List>
    {
        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            return ::boost::fusion::as_list(
                ::boost::fusion::push_front(
                    detail::std::forward<Xs>(xs),
                    detail::std::forward<X>(x)));
        }

        static auto nil_impl()
        { return ::boost::fusion::list<>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP
