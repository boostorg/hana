/*!
@file
Defines `boost::hana::ext::std::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_HPP
#define BOOST_HANA_EXT_STD_TUPLE_HPP

#include <boost/hana/fwd/ext/std/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>

// instances
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/searchable.hpp>

#include <tuple>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Functor(ext::std::Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct transform_impl<ext::std::Tuple> {
        template <typename Xs, typename F, detail::std::size_t ...index>
        static constexpr decltype(auto)
        transform_helper(Xs&& xs, F&& f, detail::std::index_sequence<index...>) {
            return ::std::make_tuple(
                f(::std::get<index>(detail::std::forward<Xs>(xs)))...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = ::std::tuple_size<Raw>::value;
            return transform_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(ext::std::Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct foldl_impl<ext::std::Tuple>
        : Iterable::foldl_impl<ext::std::Tuple>
    { };

    template <>
    struct foldr_impl<ext::std::Tuple>
        : Iterable::foldr_impl<ext::std::Tuple>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Searchable(ext::std::Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct find_impl<ext::std::Tuple>
        : Iterable::find_impl<ext::std::Tuple>
    { };

    template <>
    struct any_impl<ext::std::Tuple>
        : Iterable::any_impl<ext::std::Tuple>
    { };

    template <>
    struct Iterable::instance<ext::std::Tuple> : Iterable::mcd {
        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return ::std::get<0>(detail::std::forward<Xs>(xs));
        }

        template <typename Xs, detail::std::size_t ...index>
        static constexpr decltype(auto)
        tail_helper(Xs&& xs, detail::std::index_sequence<index...>) {
            return ::std::make_tuple(
                ::std::get<index + 1>(detail::std::forward<Xs>(xs))...
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = ::std::tuple_size<Raw>::value;
            return tail_helper(
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<N - 1>{}
            );
        }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(::std::tuple<Xs...> const&)
        { return bool_<sizeof...(Xs) == 0>; }

        template <typename N, typename Xs>
        static constexpr decltype(auto) at_impl(N n, Xs&& xs) {
            constexpr detail::std::size_t index = value(n);
            return ::std::get<index>(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct List::instance<ext::std::Tuple> : List::mcd<ext::std::Tuple> {
        static constexpr auto nil_impl()
        { return ::std::tuple<>{}; }

        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            return ::std::tuple_cat(
                ::std::make_tuple(detail::std::forward<X>(x)),
                detail::std::forward<Xs>(xs)
            );
        }

        template <typename ...Xs>
        static constexpr decltype(auto) make_impl(Xs&& ...xs) {
            return ::std::make_tuple(detail::std::forward<Xs>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Monad(ext::std::Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct flatten_impl<ext::std::Tuple> {
        template <typename ...Tuples, detail::std::size_t ...Index>
        static constexpr auto helper(::std::tuple<Tuples...> tuples,
                                     detail::std::index_sequence<Index...>)
        { return ::std::tuple_cat(::std::get<Index>(tuples)...); }

        template <typename ...Tuples>
        static constexpr decltype(auto) apply(::std::tuple<Tuples...> tuples)
        { return helper(tuples, detail::std::index_sequence_for<Tuples...>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_HPP
