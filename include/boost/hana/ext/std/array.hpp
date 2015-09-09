/*!
@file
Adapts `std::array` for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_HPP
#define BOOST_HANA_EXT_STD_ARRAY_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/less.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/integral_constant.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct array_tag; }}

    template <typename T, std::size_t N>
    struct tag_of<std::array<T, N>> {
        using type = ext::std::array_tag;
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct length_impl<ext::std::array_tag> {
        template <typename Xs>
        static constexpr auto apply(Xs const&) {
            return hana::size_c< ::std::tuple_size<Xs>::type::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<ext::std::array_tag> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t n = N::value;
            return static_cast<Xs&&>(xs)[n];
        }
    };

    template <>
    struct tail_impl<ext::std::array_tag> {
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
    struct is_empty_impl<ext::std::array_tag> {
        template <typename T, std::size_t N>
        static constexpr auto apply(std::array<T, N> const&) {
            return hana::bool_c<N == 0>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::array_tag, ext::std::array_tag> {
        template <typename T, std::size_t n, typename U>
        static constexpr bool apply(std::array<T, n> const& xs, std::array<U, n> const& ys)
        { return xs == ys; }

        template <typename T, typename U>
        static constexpr auto apply(std::array<T, 0> const&, std::array<U, 0> const&)
        { return hana::true_c; }

        template <typename T, std::size_t n, typename U, std::size_t m>
        static constexpr auto apply(std::array<T, n> const&, std::array<U, m> const&)
        { return hana::false_c; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<ext::std::array_tag, ext::std::array_tag> {
        template <typename T, std::size_t n, typename U>
        static constexpr bool apply(std::array<T, n> const& xs, std::array<U, n> const& ys)
        { return xs < ys; }

        template <typename T, typename U>
        static constexpr auto apply(std::array<T, 0> const&, std::array<U, 0> const&)
        { return hana::false_c; }

        template <typename T, std::size_t n, typename U, std::size_t m>
        static constexpr auto apply(std::array<T, n> const& xs, std::array<U, m> const& ys) {
            return detail::lexicographical_compare(
                xs.begin(), xs.end(), ys.begin(), ys.end()
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_HPP
