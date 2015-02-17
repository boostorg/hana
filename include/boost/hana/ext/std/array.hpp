/*!
@file
Defines `boost::hana::ext::std::Array`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_HPP
#define BOOST_HANA_EXT_STD_ARRAY_HPP

#include <boost/hana/fwd/ext/std/array.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/common_type.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/searchable.hpp>

#include <array>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct foldl_impl<ext::std::Array>
        : Iterable::foldl_impl<ext::std::Array>
    { };

    template <>
    struct foldr_impl<ext::std::Array>
        : Iterable::foldr_impl<ext::std::Array>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<ext::std::Array>
        : Iterable::find_impl<ext::std::Array>
    { };

    template <>
    struct any_impl<ext::std::Array>
        : Iterable::any_impl<ext::std::Array>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<ext::std::Array> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return detail::std::forward<Xs>(xs)[0]; }
    };

    template <>
    struct tail_impl<ext::std::Array> {
        template <typename T, detail::std::size_t N, typename Xs, detail::std::size_t ...index>
        static constexpr auto tail_helper(Xs&& xs, detail::std::index_sequence<index...>) {
            return ::std::array<T, N - 1>{{
                detail::std::forward<Xs>(xs)[index + 1]...
            }};
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using RawArray = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = ::std::tuple_size<RawArray>::value;
            using T = typename RawArray::value_type;
            return tail_helper<T, N>(
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<N - 1>{}
            );
        }
    };

    template <>
    struct is_empty_impl<ext::std::Array> {
        template <typename T, detail::std::size_t N>
        static constexpr auto apply(::std::array<T, N> const&)
        { return bool_<N == 0>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<ext::std::Array> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x) {
            using T = typename detail::std::decay<X>::type;
            return ::std::array<T, 1>{{detail::std::forward<X>(x)}};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<ext::std::Array> {
        template <typename T, typename Xs, typename Ys,
        detail::std::size_t ...i, detail::std::size_t ...j>
        static constexpr auto concat_helper(Xs&& xs, Ys&& ys,
                                            detail::std::index_sequence<i...>,
                                            detail::std::index_sequence<j...>)
        {
            return ::std::array<T, sizeof...(i) + sizeof...(j)>{{
                detail::std::forward<Xs>(xs)[i]...,
                detail::std::forward<Ys>(ys)[j]...
            }};
        }

        template <typename Xs, typename Ys,
            typename RawXs = typename detail::std::remove_reference<Xs>::type,
            typename RawYs = typename detail::std::remove_reference<Ys>::type,
            detail::std::size_t xs_len = ::std::tuple_size<RawXs>::value,
            detail::std::size_t ys_len = ::std::tuple_size<RawYs>::value,
            typename = detail::std::enable_if_t<xs_len != 0 && ys_len != 0>
        >
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            using T = typename detail::std::common_type<
                typename RawXs::value_type,
                typename RawYs::value_type
            >::type;

            return concat_helper<T>(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys),
                detail::std::make_index_sequence<xs_len>{},
                detail::std::make_index_sequence<ys_len>{}
            );
        }

        template <typename T, typename Ys>
        static constexpr Ys apply(::std::array<T, 0>, Ys&& ys)
        { return detail::std::forward<Ys>(ys); }

        template <typename T, typename U>
        static constexpr auto apply(::std::array<T, 0>, ::std::array<U, 0>) {
            using C = typename detail::std::common_type<T, U>::type;
            return ::std::array<C, 0>{};
        }

        template <typename T, typename Xs>
        static constexpr Xs apply(Xs&& xs, ::std::array<T, 0>)
        { return detail::std::forward<Xs>(xs); }
    };

    template <>
    struct nil_impl<ext::std::Array> {
        struct anything { };
        static constexpr auto apply()
        { return ::std::array<anything, 0>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_HPP
