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
#include <boost/hana/config.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/constexpr/algorithm.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/optional.hpp>
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
        constexpr _string<S::get()[N]...>
        prepare_impl(S, detail::std::index_sequence<N...>)
        { return {}; }

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

#ifdef BOOST_HANA_CONFIG_ENABLE_STRING_UDL
    //////////////////////////////////////////////////////////////////////////
    // _s user-defined literal
    //////////////////////////////////////////////////////////////////////////
    namespace literals {
        template <typename CharT, CharT ...s>
        constexpr auto operator"" _s() {
            static_assert(detail::std::is_same<CharT, char>::value,
            "Hana String: Only narrow string literals are supported with "
            "the _s string literal right now. See https://goo.gl/fBbKD7 "
            "if you need support for fancier types of compile-time Strings.");
            return hana::string<s...>;
        }
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <>
        struct of<String>
            : operators::of<Comparable, Orderable, Iterable>
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // to<char const*>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct to_impl<char const*, String> {
        template <char ...c>
        static constexpr char const c_string[sizeof...(c) + 1] = {c..., '\0'};

        template <char ...c>
        static constexpr char const* apply(_string<c...> const&)
        { return c_string<c...>; }
    };

    template <char ...c>
    constexpr char const to_impl<char const*, String>::c_string[sizeof...(c) + 1];

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
        template <char ...s1, char ...s2>
        static constexpr auto
        apply(_string<s1...> const&, _string<s2...> const&) {
            // We put a '\0' at the end only to avoid empty arrays.
            constexpr char const c_str1[] = {s1..., '\0'};
            constexpr char const c_str2[] = {s2..., '\0'};
            return bool_<detail::constexpr_::lexicographical_compare(
                c_str1, c_str1 + sizeof...(s1),
                c_str2, c_str2 + sizeof...(s2)
            )>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<String> {
        template <char ...s, typename F>
        static constexpr decltype(auto) apply(_string<s...> const&, F&& f)
        { return static_cast<F&&>(f)(_char<s>{}...); }
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
        template <char ...s, typename N>
        static constexpr auto apply(_string<s...> const&, N const&) {
            // We put a '\0' at the end to avoid an empty array.
            constexpr char characters[] = {s..., '\0'};
            constexpr auto n = hana::value<N>();
            return char_<characters[n]>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<String>
        : Iterable::find_if_impl<String>
    { };

    template <>
    struct contains_impl<String> {
        template <char ...s, typename C>
        static constexpr auto
        helper(_string<s...> const&, C const&, decltype(true_)) {
            constexpr char const characters[] = {s..., '\0'};
            constexpr char c = hana::value<C>();
            return bool_<
                detail::constexpr_::find(characters, characters + sizeof...(s), c)
                    != characters + sizeof...(s)
            >;
        }

        template <typename S, typename C>
        static constexpr auto helper(S const&, C const&, decltype(false_))
        { return false_; }

        template <typename S, typename C>
        static constexpr auto apply(S const& s, C const& c)
        { return helper(s, c, _models<Constant, C>{}); }
    };

    template <>
    struct find_impl<String> {
        template <char ...s, typename Char>
        static constexpr auto apply(_string<s...> const& str, Char const& c) {
            return hana::if_(hana::contains(str, c),
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
