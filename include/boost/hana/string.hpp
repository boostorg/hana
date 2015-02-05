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
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
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
        : operators::enable_adl
        , operators::Iterable_ops<_string<s...>>
    { };

    template <char ...s>
    struct datatype<_string<s...>> {
        using type = String;
    };

    //////////////////////////////////////////////////////////////////////////
    // BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
#define BOOST_HANA_STRING(s)                                                \
    (::boost::hana::to< ::boost::hana::String>([]{                          \
        struct tmp {                                                        \
            static constexpr char const* get()                              \
            { return s; }                                                   \
            using hana = tmp;                                               \
            using datatype = ::boost::hana::detail::CanonicalConstant<      \
                char const*                                                 \
            >;                                                              \
        };                                                                  \
        return tmp{};                                                       \
    }()))                                                                   \
/**/

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct enabled_operators<String>
        : Comparable, Orderable, Iterable
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(String)>
        : detail::std::true_type
    { };

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
    struct models<Orderable(String)>
        : detail::std::true_type
    { };

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
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Constant(String)>
        : detail::std::true_type
    { };

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

    template <typename C>
    struct to_impl<String, C, when<
        models<Constant(C)>{} &&
        detail::std::is_same<typename C::value_type, char const*>{}
    >> : embedding<> {
        template <typename S, detail::std::size_t ...i>
        static constexpr auto helper(S, detail::std::index_sequence<i...>)
        { return string<value2<S>()[i]...>; }

        static constexpr detail::std::size_t strlen(char const* s) {
            detail::std::size_t len = 0;
            while (*s++ != '\0') ++len;
            return len;
        }

        template <typename S>
        static constexpr decltype(auto) apply(S s) {
            constexpr detail::std::size_t len = strlen(value2<S>());
            return helper(s, detail::std::make_index_sequence<len>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(String)>
        : detail::std::true_type
    { };

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
    struct models<Iterable(String)>
        : detail::std::true_type
    { };

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
    struct models<Searchable(String)>
        : detail::std::true_type
    { };

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

        template <char ...s, typename C>
        static constexpr auto apply(_string<s...>, C character) {
            constexpr char c = hana::value(character);
            constexpr char c_str[] = {s..., '\0'};
            return bool_<str_elem(c_str, c)>;
        }
    };

    template <>
    struct lookup_impl<String> {
        template <char ...s, typename C>
        static constexpr auto apply(_string<s...> str, C character) {
            return hana::if_(hana::elem(str, character),
                hana::just(character),
                nothing
            );
        }
    };

    template <>
    struct any_impl<String>
        : Iterable::any_impl<String>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_HPP
