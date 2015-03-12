/*!
@file
Defines `boost::hana::String`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_HPP
#define BOOST_HANA_STRING_HPP

#include <boost/hana/fwd/string.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////
    template <char ...s>
    struct _string
        : operators::adl
        , operators::Iterable_ops<_string<s...>>
    { };

    template <char ...s>
    struct datatype<_string<s...>> {
        using type = String;
    };

    //////////////////////////////////////////////////////////////////////////
    // BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
    namespace string_detail {
        template <typename S, detail::std::size_t ...N>
        constexpr decltype(auto)
        prepare_impl(S, detail::std::index_sequence<N...>)
        { return string<S::get()[N]...>; }

        template <typename S>
        constexpr decltype(auto) prepare(S s) {
            return prepare_impl(s,
                detail::std::make_index_sequence<sizeof(S::get()) - 1>{});
        }
    }

#define BOOST_HANA_STRING(s)                                                \
    (::boost::hana::string_detail::prepare([]{                              \
        struct tmp {                                                        \
            static constexpr decltype(auto) get() { return s; }             \
        };                                                                  \
        return tmp{};                                                       \
    }()))                                                                   \
/**/

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<String>
        : operators::of<Comparable, Orderable, Iterable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<String, String> {
        template <typename S>
        static constexpr auto apply(S const&, S const&)
        { return true_; }

        template <typename S1, typename S2>
        static constexpr auto apply(S1 const&, S2 const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
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

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
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

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
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
            constexpr auto i = hana::value(index);
            return char_<characters[i]>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<String>
        : Iterable::find_impl<String>
    { };

    template <>
    struct elem_impl<String> {
        static constexpr bool str_elem(char const* s, char c) {
            while (*s != '\0')
                if (*s++ == c)
                    return true;
            return false;
        }

        template <char ...s, typename Char>
        static constexpr auto
        helper(_string<s...>, Char c, decltype(true_)) {
            constexpr char c_str[] = {s..., '\0'};
            return bool_<str_elem(c_str, hana::value(c))>;
        }

        template <typename S, typename Char>
        static constexpr auto helper(S, Char, decltype(false_))
        { return false_; }

        template <typename S, typename Char>
        static constexpr decltype(auto) apply(S s, Char c) {
            return helper(s, c, models<Constant, Char>);
        }
    };

    template <>
    struct lookup_impl<String> {
        template <char ...s, typename Char>
        static constexpr auto apply(_string<s...> str, Char c) {
            return hana::if_(hana::elem(str, c),
                hana::just(c),
                nothing
            );
        }
    };

    template <>
    struct any_of_impl<String>
        : Iterable::any_of_impl<String>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_HPP
