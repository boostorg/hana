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
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral.hpp>

//! @todo
//! This enables conversion from any Foldable to a List, so String -> List
//! works.
#include <boost/hana/list.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `String`s.
    //!
    //! Two `String`s are equal if and only if each of their
    //! characters are equal.
    //!
    //! ### Example
    //! @snippet example/string/comparable.cpp main
    template <>
    struct equal_impl<String, String> {
        template <typename S>
        static constexpr auto apply(S const&, S const&)
        { return true_; }

        template <typename S1, typename S2>
        static constexpr auto apply(S1 const&, S2 const&)
        { return false_; }
    };

    namespace string_detail {
        template <char ...s>
        constexpr char const store[sizeof...(s) + 1] = {s..., '\0'};
    }

    //! Instance of `Constant` for `String`s.
    //!
    //! A `String`'s compile-time value is a constexpr `char const*` to its
    //! internal data.
    //!
    //! ### Example
    //! @snippet example/string/constant.cpp main
    template <>
    struct value_impl<String> {
        template <char ...s>
        static constexpr char const* apply(_string<s...> const&)
        { return string_detail::store<s...>; }
    };

    //! Instance of `Foldable` for `String`s.
    //!
    //! For the purpose of being folded, `String`s are basically equivalent to
    //! a tuple in which `IntegralConstant`s of underlying type `char` are
    //! stored.
    //!
    //! ### Example
    //! @snippet example/string/foldable.cpp main
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

    //! Instance of `Iterable` for `String`s.
    //!
    //! For the purpose of being iterated over, `String`s are basically
    //! equivalent to a tuple of `IntegralConstant`s with an underlying
    //! type of `char`.
    //!
    //! ### Example
    //! @snippet example/string/iterable.cpp main
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

    //! Instance of `Orderable` for `String`s.
    //!
    //! The strict weak ordering implemented for `Orderable` is the usual
    //! lexicographical comparison.
    //!
    //! ### Example
    //! @snippet example/string/orderable.cpp main
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_HPP
