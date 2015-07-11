/*!
@file
Defines `boost::hana::ext::std::Array`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_HPP
#define BOOST_HANA_EXT_STD_ARRAY_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/value.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct Array; }}

    template <typename T, std::size_t N>
    struct datatype<std::array<T, N>> {
        using type = ext::std::Array;
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::Array, ext::std::Array>
        : Sequence::equal_impl<ext::std::Array, ext::std::Array>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<ext::std::Array, ext::std::Array>
        : Sequence::less_impl<ext::std::Array, ext::std::Array>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct fold_left_impl<ext::std::Array>
        : Iterable::fold_left_impl<ext::std::Array>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<ext::std::Array>
        : Iterable::find_if_impl<ext::std::Array>
    { };

    template <>
    struct any_of_impl<ext::std::Array>
        : Iterable::any_of_impl<ext::std::Array>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<ext::std::Array> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t n = hana::value<N>();
            return static_cast<Xs&&>(xs)[n];
        }
    };

    template <>
    struct tail_impl<ext::std::Array> {
        template <typename T, std::size_t N, typename Xs, std::size_t ...index>
        static constexpr auto tail_helper(Xs&& xs, std::index_sequence<index...>) {
            return std::array<T, N - 1>{{
                static_cast<Xs&&>(xs)[index + 1]...
            }};
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using RawArray = typename std::remove_cv<
                typename std::remove_reference<Xs>::type
            >::type;
            constexpr auto N = std::tuple_size<RawArray>::value;
            using T = typename RawArray::value_type;
            return tail_helper<T, N>(
                static_cast<Xs&&>(xs),
                std::make_index_sequence<N - 1>{}
            );
        }
    };

    template <>
    struct is_empty_impl<ext::std::Array> {
        template <typename T, std::size_t N>
        static constexpr _integral_constant<bool, N == 0>
        apply(std::array<T, N> const&) { return {}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_HPP
