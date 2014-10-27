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
#include <boost/hana/detail/std/integer_sequence.hpp>
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
    namespace string_detail {
        constexpr bool equal(char const* s1, char const* s2) {
            while (*s1 != '\0' && *s2 != '\0')
                if (*s1++ != *s2++)
                    return false;
            return *s1 == '\0' && *s2 == '\0';
        }

        constexpr detail::std::size_t length(char const* s) {
            detail::std::size_t l = 0;
            while (*s++ != '\0')
                ++l;
            return l;
        }

        constexpr bool less(char const* s1, char const* s2) {
            while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
                ++s1, ++s2;

            return (*s1 == '\0' && *s2 != '\0') || // s1 is shorter than s2
                   (*s1 != '\0' && *s2 != '\0' && *s1 < *s2); // s1[0] < s2[0]
        }
    }

    //! Instance of `Comparable` for `String`s.
    //!
    //! Two `String`s are equal if and only if each of their
    //! characters are equal.
    //!
    //! ### Example
    //! @snippet example/string/comparable.cpp main
    template <>
    struct Comparable::instance<String, String> : Comparable::equal_mcd {
        template <typename S1, typename S2>
        static constexpr auto equal_impl(S1 const&, S2 const&) {
            constexpr char const* s1 = S1::get();
            constexpr char const* s2 = S2::get();
            return bool_<string_detail::equal(s1, s2)>;
        }
    };

    //! Instance of `Constant` for `String`s.
    //!
    //! A `String`'s compile-time value is a `constexpr char*` to its
    //! internal data.
    //!
    //! ### Example
    //! @snippet example/string/constant.cpp main
    template <>
    struct Constant::instance<String> : Constant::mcd {
        template <typename S>
        static constexpr auto value_impl(S const&) {
            return S::get();
        }
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
    struct Foldable::instance<String> : Foldable::unpack_mcd {
        template <typename S, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto) unpack_helper(S, F&& f,
                                            detail::std::index_sequence<i...>)
        {
            constexpr char const* s = S::get();
            return detail::std::forward<F>(f)(char_<s[i]>...);
        }

        template <typename S, typename F>
        static constexpr decltype(auto) unpack_impl(S s, F&& f) {
            return unpack_helper(s, detail::std::forward<F>(f),
                detail::std::make_index_sequence<string_detail::length(S::get())>{}
            );
        }

        template <typename S>
        static constexpr auto length_impl(S) {
            return size_t<string_detail::length(S::get())>;
        }
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
    struct Iterable::instance<String> : Iterable::mcd {
        template <typename S>
        static constexpr auto head_impl(S) {
            return char_<S::get()[0]>;
        }

        template <typename S, detail::std::size_t ...i>
        static constexpr auto tail_helper(S, detail::std::index_sequence<i...>) {
            return string<S::get()[i+1]...>;
        }

        template <typename S>
        static constexpr auto tail_impl(S s) {
            return tail_helper(s, detail::std::make_index_sequence<
                string_detail::length(S::get()) - 1
            >{});
        }

        template <typename S>
        static constexpr auto is_empty_impl(S) {
            return bool_<S::get()[0] == '\0'>;
        }

        template <typename I, typename S>
        static constexpr auto at_impl(I index, S) {
            constexpr auto i = value(index);
            return char_<S::get()[i]>;
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
    struct Orderable::instance<String, String> : Orderable::less_mcd {
        template <typename S1, typename S2>
        static constexpr auto less_impl(S1 const&, S2 const&) {
            constexpr char const* s1 = S1::get();
            constexpr char const* s2 = S2::get();
            return bool_<string_detail::less(s1, s2)>;
        }
    };

    //! Instance of `Searchable` for `String`s.
    //!
    //! For the purpose of being searched, `String`s are basically equivalent
    //! to a tuple of `IntegralConstant`s of underlying type `char`.
    //!
    //! ### Example
    //! @snippet example/string/searchable.cpp main
    template <>
    struct Searchable::instance<String>
        : Searchable::iterable_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_HPP
