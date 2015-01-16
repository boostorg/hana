/*!
@file
Forward declares `boost::hana::String`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_STRING_HPP
#define BOOST_HANA_FWD_STRING_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/iterable.hpp>
#include <boost/hana/fwd/orderable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time string.
    //!
    //!
    //! @todo
    //! Right now, we use a different template specialization to store each
    //! different string. This could also work by using an anonymous type,
    //! but there seems to be a bug in Clang right now that makes this fail
    //! in some circumstances: http://llvm.org/bugs/show_bug.cgi?id=20625.
    //! Using an anonymous type could have compile-time performance benefits,
    //! so this avenue should be explored once the bug is fixed.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Searchable`
    //! For the purpose of being searched, `String`s are basically equivalent
    //! to a tuple of `IntegralConstant`s of underlying type `char`.
    //!
    //! ### Example
    //! @snippet example/string/searchable.cpp main
    //!
    //!
    //! 2. `Comparable`
    //! @todo
    //!
    //! 3. `Orderable`
    //! @todo
    //!
    //! 4. `Constant`
    //! @todo
    //!
    //!
    //! 5. `Foldable`
    //! @todo
    //!
    //! 6. `Iterable`
    //! @todo
    struct String {
        struct hana {
            struct enabled_operators
                : Comparable
                , Orderable
                , Iterable
            { };
        };
    };

    //! Create a compile-time string from a parameter pack of characters.
    //! @relates String
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <char ...s>
    constexpr unspecified-type string{};
#else
    template <char ...s>
    struct _string
        : operators::enable_adl
        , operators::Iterable_ops<_string<s...>>
    {
        struct hana { using datatype = String; };
    };

    template <char ...s>
    constexpr _string<s...> string{};
#endif

    namespace string_detail {
        template <typename S, detail::std::size_t ...N>
        constexpr decltype(auto)
        prepare_impl(S, detail::std::index_sequence<N...>)
        { return string<S::storage()[N]...>; }

        template <typename S>
        constexpr decltype(auto) prepare(S s) {
            return prepare_impl(s,
                detail::std::make_index_sequence<sizeof(S::storage()) - 1>{});
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

#endif // !BOOST_HANA_FWD_STRING_HPP
