/*!
@file
Defines `boost::hana::string`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_HPP
#define BOOST_HANA_STRING_HPP

#include <boost/hana/fwd/string.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/contains.hpp>
#include <boost/hana/fwd/core/convert.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/find.hpp>
#include <boost/hana/fwd/front.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/less.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>

#include <utility>
#include <cstddef>
#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // string<>
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <char ...s>
    struct string
        : operators::adl
        , detail::iterable_operators<string<s...>>
    { };
    //! @endcond

    template <char ...s>
    struct datatype<string<s...>> {
        using type = String;
    };

    //////////////////////////////////////////////////////////////////////////
    // make<String>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<String> {
        template <typename ...Chars>
        static constexpr auto apply(Chars const& ...) {
            return hana::string<hana::value<Chars>()...>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
    namespace string_detail {
        template <typename S, std::size_t ...N>
        constexpr string<S::get()[N]...>
        prepare_impl(S, std::index_sequence<N...>)
        { return {}; }

        template <typename S>
        constexpr decltype(auto) prepare(S s) {
            return prepare_impl(s,
                std::make_index_sequence<sizeof(S::get()) - 1>{});
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
            static_assert(std::is_same<CharT, char>::value,
            "hana::string: Only narrow string literals are supported with "
            "the _s string literal right now. See https://goo.gl/fBbKD7 "
            "if you need support for fancier types of compile-time strings.");
            return hana::string_c<s...>;
        }
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<String> {
            static constexpr bool value = true;
        };
        template <>
        struct orderable_operators<String> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // to<char const*>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct to_impl<char const*, String> {
        template <char ...c>
        static constexpr char const c_string[sizeof...(c) + 1] = {c..., '\0'};

        template <char ...c>
        static constexpr char const* apply(string<c...> const&)
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
        { return hana::true_c; }

        template <typename S1, typename S2>
        static constexpr auto apply(S1 const&, S2 const&)
        { return hana::false_c; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<String, String> {
        template <char ...s1, char ...s2>
        static constexpr auto
        apply(string<s1...> const&, string<s2...> const&) {
            // We put a '\0' at the end only to avoid empty arrays.
            constexpr char const c_str1[] = {s1..., '\0'};
            constexpr char const c_str2[] = {s2..., '\0'};
            return hana::bool_c<detail::lexicographical_compare(
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
        static constexpr decltype(auto) apply(string<s...> const&, F&& f)
        { return static_cast<F&&>(f)(char_<s>{}...); }
    };

    template <>
    struct length_impl<String> {
        template <char ...s>
        static constexpr auto apply(string<s...> const&)
        { return hana::size_c<sizeof...(s)>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct front_impl<String> {
        template <char x, char ...xs>
        static constexpr auto apply(string<x, xs...> const&)
        { return hana::char_c<x>; }
    };

    template <>
    struct tail_impl<String> {
        template <char x, char ...xs>
        static constexpr auto apply(string<x, xs...> const&)
        { return hana::string_c<xs...>; }
    };

    template <>
    struct is_empty_impl<String> {
        template <char ...s>
        static constexpr auto apply(string<s...> const&)
        { return hana::bool_c<sizeof...(s) == 0>; }
    };

    template <>
    struct at_impl<String> {
        template <char ...s, typename N>
        static constexpr auto apply(string<s...> const&, N const&) {
            // We put a '\0' at the end to avoid an empty array.
            constexpr char characters[] = {s..., '\0'};
            constexpr auto n = hana::value<N>();
            return hana::char_c<characters[n]>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct contains_impl<String> {
        template <char ...s, typename C>
        static constexpr auto
        helper(string<s...> const&, C const&, decltype(hana::true_c)) {
            constexpr char const characters[] = {s..., '\0'};
            constexpr char c = hana::value<C>();
            return hana::bool_c<
                detail::find(characters, characters + sizeof...(s), c)
                    != characters + sizeof...(s)
            >;
        }

        template <typename S, typename C>
        static constexpr auto helper(S const&, C const&, decltype(hana::false_c))
        { return hana::false_c; }

        template <typename S, typename C>
        static constexpr auto apply(S const& s, C const& c)
        { return helper(s, c, _models<Constant, C>{}); }
    };

    template <>
    struct find_impl<String> {
        template <char ...s, typename Char>
        static constexpr auto apply(string<s...> const& str, Char const& c) {
            return hana::if_(hana::contains(str, c),
                hana::just(c),
                hana::nothing
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_HPP
