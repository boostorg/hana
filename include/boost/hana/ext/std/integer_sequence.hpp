/*!
@file
Adapts `std::integer_sequence` for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/fwd/unpack.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct integer_sequence_tag; }}

    template <typename T, T ...v>
    struct tag_of<std::integer_sequence<T, v...>> {
        using type = ext::std::integer_sequence_tag;
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::integer_sequence_tag, ext::std::integer_sequence_tag> {
        template <typename X, X ...xs, typename Y, Y ...ys>
        static constexpr auto apply(
           std::integer_sequence<X, xs...>,
           std::integer_sequence<Y, ys...>,
            // this dummy parameter disables this specialization if
            // sizeof...(xs) != sizeof...(ys)
            char(*)[sizeof...(xs) == sizeof...(ys)] = 0)
        {
            return hana::bool_c<std::is_same<
               std::integer_sequence<bool, (xs == ys)...>,
               std::integer_sequence<bool, ((void)xs, true)...>
            >::value>;
        }

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs, Ys, ...)
        { return hana::false_c; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<ext::std::integer_sequence_tag> {
        template <typename T, T ...v, typename F>
        static constexpr decltype(auto)
        apply(std::integer_sequence<T, v...>, F&& f) {
            return static_cast<F&&>(f)(
                std::integral_constant<T, v>{}...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<ext::std::integer_sequence_tag> {
        template <typename T, T ...v, typename N>
        static constexpr auto apply(std::integer_sequence<T, v...> const&, N const&) {
            constexpr std::size_t n = N::value;
            constexpr T values[] = {v...};
            return std::integral_constant<T, values[n]>{};
        }
    };

    template <>
    struct tail_impl<ext::std::integer_sequence_tag> {
        template <typename T, T x, T ...xs>
        static constexpr auto apply(std::integer_sequence<T, x, xs...>)
        { return std::integer_sequence<T, xs...>{}; }
    };

    template <>
    struct is_empty_impl<ext::std::integer_sequence_tag> {
        template <typename T, T ...xs>
        static constexpr auto apply(std::integer_sequence<T, xs...>)
        { return hana::bool_c<sizeof...(xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
