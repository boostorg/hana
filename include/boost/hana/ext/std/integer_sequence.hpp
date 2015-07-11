/*!
@file
Defines `boost::hana::ext::std::IntegerSequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct IntegerSequence; }}

    template <typename T, T ...v>
    struct datatype<std::integer_sequence<T, v...>> {
        using type = ext::std::IntegerSequence;
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::IntegerSequence, ext::std::IntegerSequence> {
        template <typename X, X ...xs, typename Y, Y ...ys>
        static constexpr auto apply(
           std::integer_sequence<X, xs...>,
           std::integer_sequence<Y, ys...>,
            // this dummy parameter disables this specialization if
            // sizeof...(xs) != sizeof...(ys)
            char(*)[sizeof...(xs) == sizeof...(ys)] = 0)
        {
            return bool_<std::is_same<
               std::integer_sequence<bool, (xs == ys)...>,
               std::integer_sequence<bool, ((void)xs, true)...>
            >::value>;
        }

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs, Ys, ...)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<ext::std::IntegerSequence> {
        template <typename T, T ...v, typename F>
        static constexpr decltype(auto)
        apply(std::integer_sequence<T, v...>, F&& f) {
            return static_cast<F&&>(f)(
                std::integral_constant<T, v>{}...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<ext::std::IntegerSequence>
        : Iterable::find_if_impl<ext::std::IntegerSequence>
    { };

    template <>
    struct any_of_impl<ext::std::IntegerSequence>
        : Iterable::any_of_impl<ext::std::IntegerSequence>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<ext::std::IntegerSequence> {
        template <typename T, T ...v, typename N>
        static constexpr auto apply(std::integer_sequence<T, v...> const&, N const&) {
            constexpr std::size_t n = hana::value<N>();
            constexpr T values[] = {v...};
            return std::integral_constant<T, values[n]>{};
        }
    };

    template <>
    struct tail_impl<ext::std::IntegerSequence> {
        template <typename T, T x, T ...xs>
        static constexpr auto apply(std::integer_sequence<T, x, xs...>)
        { return std::integer_sequence<T, xs...>{}; }
    };

    template <>
    struct is_empty_impl<ext::std::IntegerSequence> {
        template <typename T, T ...xs>
        static constexpr auto apply(std::integer_sequence<T, xs...>)
        { return bool_<sizeof...(xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
