/*!
@file
Defines `boost::hana::String`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_HPP
#define BOOST_HANA_STRING_HPP

#include <boost/hana/fwd/string.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    // Comparable
    template <>
    struct equal_impl<String, String> {
        template <typename S>
        static constexpr auto apply(S const&, S const&)
        { return true_; }

        template <typename S1, typename S2>
        static constexpr auto apply(S1 const&, S2 const&)
        { return false_; }
    };

    // Orderable
    template <>
    struct less_impl<String, String> {
        static constexpr bool less_helper(char const* s1, char const* s2) {
            while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
                ++s1, ++s2;

            return (*s1 == '\0' && *s2 != '\0') || // s1 is shorter than s2
                   (*s1 != '\0' && *s2 != '\0' && *s1 < *s2); // s1[0] < s2[0]
        }

        template <char ...s1, char ...s2>
        static constexpr auto
        apply(_string<s1...> const&, _string<s2...> const&) {
            constexpr char const c_str1[] = {s1..., '\0'};
            constexpr char const c_str2[] = {s2..., '\0'};
            return bool_<less_helper(c_str1, c_str2)>;
        }
    };

    // Constant
    namespace string_detail {
        template <char ...s>
        constexpr char const store[sizeof...(s) + 1] = {s..., '\0'};
    }

    template <>
    struct value_impl<String> {
        template <char ...s>
        static constexpr char const* apply(_string<s...> const&)
        { return string_detail::store<s...>; }
    };

    // Foldable
    template <>
    struct unpack_impl<String> {
        template <char ...s, typename F>
        static constexpr decltype(auto) apply(_string<s...> const&, F&& f)
        { return detail::std::forward<F>(f)(char_<s>...); }
    };

    template <>
    struct length_impl<String> {
        template <char ...s>
        static constexpr auto apply(_string<s...> const&)
        { return size_t<sizeof...(s)>; }
    };

    // Iterable
    template <>
    struct head_impl<String> {
        template <char x, char ...xs>
        static constexpr auto apply(_string<x, xs...> const&)
        { return char_<x>; }
    };

    template <>
    struct tail_impl<String> {
        template <char x, char ...xs>
        static constexpr auto apply(_string<x, xs...> const&)
        { return string<xs...>; }
    };

    template <>
    struct is_empty_impl<String> {
        template <char ...s>
        static constexpr auto apply(_string<s...> const&)
        { return bool_<sizeof...(s) == 0>; }
    };

    template <>
    struct at_impl<String> {
        template <typename I, char ...s>
        static constexpr auto apply(I index, _string<s...> const&) {
            constexpr char characters[] = {s...};
            constexpr auto i = value(index);
            return char_<characters[i]>;
        }
    };

    // Searchable
    /* provided by Iterable */

}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_HPP
