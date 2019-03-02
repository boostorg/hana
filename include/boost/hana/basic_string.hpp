/*!
@file
Defines `boost::hana::basic_string`.

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BASIC_STRING_HPP
#define BOOST_HANA_BASIC_STRING_HPP

#include <boost/hana/fwd/basic_string.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/contains.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/core/to.hpp>
#include <boost/hana/fwd/drop_front.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/find.hpp>
#include <boost/hana/fwd/front.hpp>
#include <boost/hana/fwd/hash.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/less.hpp>
#include <boost/hana/fwd/plus.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/fwd/zero.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/type.hpp>

#include <utility>
#include <cstddef>
#include <type_traits>


BOOST_HANA_NAMESPACE_BEGIN
    //////////////////////////////////////////////////////////////////////////
    // basic_string<CharT>
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    namespace detail {
        template <typename CharT, CharT ...s>
        constexpr CharT const basic_string_storage[sizeof...(s) + 1] = {
          s..., static_cast<CharT>('\0')
        };
    }

    template <typename CharT, CharT ...s>
    struct basic_string
        : detail::operators::adl<basic_string<CharT, s...>>
        , detail::iterable_operators<basic_string<CharT, s...>>
    {
        //using value_type = CharT;
        static constexpr CharT const* c_str() {
            return &detail::basic_string_storage<CharT, s...>[0];
        }
    };
    //! @endcond

    template <typename CharT, CharT ...s>
    struct tag_of<basic_string<CharT, s...>> {
        using type = basic_string_tag<CharT>;
    };

    //////////////////////////////////////////////////////////////////////////
    // make<basic_string_tag>
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct make_impl<basic_string_tag<CharT>> {
        template <typename ...Chars>
        static constexpr auto apply(Chars const& ...) {
            return hana::basic_string<CharT, hana::value<Chars>()...>();
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
    namespace basic_string_detail {
        template <typename CharTo, typename CharFrom>
        struct CastChar {
            template <CharFrom Value>
            constexpr static CharTo apply = static_cast<CharTo>(
                static_cast<std::make_unsigned_t<CharFrom>>(Value)
            );
        };
        template <typename CharT>
        struct CastChar<CharT, CharT> {
            template <CharT Value>
            constexpr static CharT apply = Value;
        };

        template <typename Cast, typename S, std::size_t ...N>
        constexpr auto
        prepare_impl(S, std::integer_sequence<std::size_t, N...>)
            -> basic_string<
                std::decay_t<decltype(Cast::template apply<*S::get()>)>,
                Cast::template apply<S::get()[N]>...>
        { return {}; }

        template <typename CharTo, typename CharFrom, typename S>
        constexpr decltype(auto) prepare(S s) {
            return prepare_impl<CastChar<CharTo, CharFrom>>(
                s,
                std::make_index_sequence<
                        sizeof(S::get())/sizeof(decltype(*S::get())) - 1
                     >{}
            );
        }
    }

#define BOOST_HANA_AUTO_STRING(s)                                   \
    (::boost::hana::basic_string_detail::prepare<                   \
         std::decay_t<decltype(*s)>, std::decay_t<decltype(*s)>     \
       >([]{                                                        \
           struct tmp {                                             \
               static constexpr decltype(auto) get() { return s; }  \
           };                                                       \
           return tmp{};                                            \
        }()))

#define BOOST_HANA_BASIC_STRING(type, s)                            \
    (::boost::hana::basic_string_detail::prepare<                   \
         type, std::decay_t<decltype(*s)>                           \
       >([]{                                                        \
           struct tmp {                                             \
               static constexpr decltype(auto) get() { return s; }  \
           };                                                       \
           return tmp{};                                            \
         }()))
/**/

#ifdef BOOST_HANA_CONFIG_ENABLE_BASIC_STRING_UDL
    //////////////////////////////////////////////////////////////////////////
    // _s user-defined literal
    //////////////////////////////////////////////////////////////////////////
    namespace literals {
        template <typename CharT, CharT ...s>
        constexpr auto operator"" _s() {
            return hana::basic_string_c<CharT, s...>;
        }
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename CharT>
        struct comparable_operators<basic_string_tag<CharT>> {
            static constexpr bool value = true;
        };
        template <typename CharT>
        struct orderable_operators<basic_string_tag<CharT>> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // to<char const*>
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct to_impl<CharT const*, basic_string_tag<CharT>> {
        template <CharT ...c>
        static constexpr CharT const* apply(basic_string<CharT, c...> const&)
        { return basic_string<CharT, c...>::c_str(); }
    };

    //////////////////////////////////////////////////////////////////////////
    // to<string_tag>
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename CharT>
        constexpr std::size_t cx_bs_strlen(CharT const* s) {
            std::size_t n = 0u;
            while (*s != static_cast<CharT>('\0'))
                ++s, ++n;
            return n;
        }

        template <typename CharT, typename S, std::size_t ...I>
        constexpr hana::basic_string<CharT, hana::value<S>()[I]...>
        bs_expand(std::index_sequence<I...>)
        { return {}; }
    }

    template <typename CharT, typename IC>
    struct to_impl<hana::basic_string_tag<CharT>, IC, hana::when<
        hana::Constant<IC>::value &&
        std::is_convertible<typename IC::value_type, CharT const*>::value
    >> {
        template <typename S>
        static constexpr auto apply(S const&) {
            constexpr CharT const* s = hana::value<S>();
            constexpr std::size_t len = detail::cx_bs_strlen<CharT>(s);
            return detail::bs_expand<CharT, S>(std::make_index_sequence<len>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct equal_impl<basic_string_tag<CharT>, basic_string_tag<CharT>> {
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
    template <typename CharT>
    struct less_impl<basic_string_tag<CharT>, basic_string_tag<CharT>> {
        template <CharT ...s1, CharT ...s2>
        static constexpr auto
        apply(basic_string<CharT, s1...> const&,
              basic_string<CharT, s2...> const&) {
            constexpr CharT const c_str1[] = { s1...,
                                               static_cast<CharT>('\0') };
            constexpr CharT const c_str2[] = { s2...,
                                               static_cast<CharT>('\0') };
            return hana::bool_c<detail::lexicographical_compare(
                c_str1, c_str1 + sizeof...(s1),
                c_str2, c_str2 + sizeof...(s2)
            )>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monoid
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct plus_impl<basic_string_tag<CharT>, basic_string_tag<CharT>> {
        template <CharT ...s1, CharT ...s2>
        static constexpr auto
        apply(basic_string<CharT, s1...> const&,
              basic_string<CharT, s2...> const&) {
            return basic_string<CharT, s1..., s2...>{};
        }
    };

    template <typename CharT>
    struct zero_impl<basic_string_tag<CharT>> {
        static constexpr auto apply() {
            return basic_string<CharT>{};
        }
    };

    template <typename CharT, CharT ...s1, CharT ...s2>
    constexpr auto operator+(
        basic_string<CharT, s1...> const&, basic_string<CharT, s2...> const&
    ) {
        return hana::basic_string<CharT, s1..., s2...>{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct unpack_impl<basic_string_tag<CharT>> {
        template <CharT ...s, typename F>
        static constexpr decltype(auto) apply(
            basic_string<CharT, s...> const&, F&&f
        ) {
            return static_cast<F&&>(f)(basic_char_<CharT, s>{}...);
        }
    };

    template <typename CharT>
    struct length_impl<basic_string_tag<CharT>> {
        template <CharT ...s>
        static constexpr auto apply(basic_string<CharT, s...> const&) {
            return hana::size_c<sizeof...(s)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct front_impl<basic_string_tag<CharT>> {
        template <CharT x, CharT ...xs>
        static constexpr auto apply(basic_string<CharT, x, xs...> const&) {
            return hana::basic_char_c<CharT, x>;
        }
    };

    template <typename CharT>
    struct drop_front_impl<basic_string_tag<CharT>> {
        template <std::size_t N, CharT ...xs, std::size_t ...i>
        static constexpr auto helper(
            basic_string<CharT, xs...> const&, std::index_sequence<i...>
        ) {
            constexpr CharT s[] = { xs... };
            return hana::basic_string_c<CharT, s[i + N]...>;
        }

        template <CharT ...xs, typename N>
        static constexpr auto apply(
            basic_string<CharT, xs...> const& s, N const&
        ) {
            return helper<N::value>(s, std::make_index_sequence<
                N::value < sizeof...(xs) ? sizeof...(xs) - N::value : 0>{});
        }

        template <typename N>
        static constexpr auto apply(basic_string<CharT> const& s, N const&) {
            return s;
        }
    };

    template <typename CharT>
    struct is_empty_impl<basic_string_tag<CharT>> {
        template <CharT ...s>
        static constexpr auto apply(basic_string<CharT, s...> const&) {
            return hana::bool_c<sizeof...(s) == 0>;
        }
    };

    template <typename CharT>
    struct at_impl<basic_string_tag<CharT>> {
        template <CharT ...s, typename N>
        static constexpr auto apply(
            basic_string<CharT, s...> const&, N const&
        ) {
            constexpr CharT characters[] = { s..., static_cast<CharT>('\0') };
            constexpr auto n = N::value;
            return hana::basic_char_c<CharT, characters[n]>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct contains_impl<basic_string_tag<CharT>> {
        template <CharT ...s, typename C>
        static constexpr auto
        helper(basic_string<CharT, s...> const&, C const&, hana::true_) {
            constexpr CharT const characters[] = { s...,
                                                   static_cast<CharT>('\0') };
            constexpr CharT c = hana::value<C>();
            return hana::bool_c<
                detail::find(characters, characters + sizeof...(s), c)
                != characters + sizeof...(s)
                >;
        }

        template <typename S, typename C>
        static constexpr auto helper(S const&, C const&, hana::false_) {
            return hana::false_c;
        }

        template <typename S, typename C>
        static constexpr auto apply(S const& s, C const& c) {
            return helper(s, c, hana::bool_c<hana::Constant<C>::value>);
        }
    };

    template <typename CharT>
    struct find_impl<basic_string_tag<CharT>> {
        template <CharT ...s, typename Ch>
        static constexpr auto apply(
          basic_string<CharT, s...> const& str, Ch const& c
        ) {
            return hana::if_(
                contains_impl<basic_string_tag<CharT>>::apply(str, c),
                hana::just(c),
                hana::nothing
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Hashable
    //////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct hash_impl<basic_string_tag<CharT>> {
        template <typename String>
        static constexpr auto apply(String const&) {
            return hana::type_c<String>;
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_BASIC_STRING_HPP
