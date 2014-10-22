/*!
@file
Forward declares `boost::hana::String`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_STRING_HPP
#define BOOST_HANA_STRING_STRING_HPP

#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/iterable/operators.hpp>
#include <boost/hana/orderable/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time string.
    //!
    //! ## Instance of
    //! `Comparable`, `Orderable`, `Constant`, `Foldable`, `Iterable`, and
    //! `Searchable`
    //!
    //! @todo
    //! Right now, we use a different template specialization to store each
    //! different string. This could also work by using an anonymous type,
    //! but there seems to be a bug in Clang right now that makes this fail
    //! in some circumstances: http://llvm.org/bugs/show_bug.cgi?id=20625.
    //! Using an anonymous type could have compile-time performance benefits,
    //! so this avenue should be explored once the bug is fixed.
    struct String {
        struct hana_enabled_operators
            : Comparable, Orderable, Iterable
        { };
    };

    namespace string_detail {
        template <char ...s>
        struct string
            : operators::enable_adl
            , operators::Iterable_ops<string<s...>>
        {
            static constexpr char const storage[sizeof...(s)+1] = {s..., '\0'};
            static constexpr char const* get() { return storage; }
            using hana_datatype = String;
        };
    }

    //! Create a compile-time string from a parameter pack of characters.
    //! @relates String
    template <char ...s>
    constexpr string_detail::string<s...> string{};

    namespace string_detail {
        template <typename S, detail::std::size_t ...N>
        constexpr decltype(auto) prepare_impl(S, detail::std::index_sequence<N...>)
        { return hana::string<S::storage()[N]...>; }

        template <typename S>
        constexpr decltype(auto) prepare(S s) {
            return prepare_impl(
                s, detail::std::make_index_sequence<sizeof(S::storage()) - 1>{}
            );
        }
    }

    //! Create a compile-time string from a string literal `s`.
    //! @relates String
    //!
    //! This macro is a more convenient alternative to `string` for creating
    //! compile-time strings. However, since this macro uses a lambda
    //! internally, it can't be used in an unevaluated context.
    //!
    //! ### Example
    //! @snippet example/string/string.cpp macro
    //!
    //! @todo
    //! This could be generalized to arbitrary objects. This is basically a
    //! way to create a `Constant` from any `constexpr` object.
#define BOOST_HANA_STRING(s)                                                \
    (::boost::hana::string_detail::prepare([]{                              \
        struct tmp {                                                        \
            static constexpr decltype(auto) storage() { return s; }         \
        };                                                                  \
        return tmp{};                                                       \
    }()))                                                                   \
/**/
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_STRING_HPP
