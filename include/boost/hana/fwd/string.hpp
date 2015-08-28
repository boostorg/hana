/*!
@file
Forward declares `boost::hana::String`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_STRING_HPP
#define BOOST_HANA_FWD_STRING_HPP

#include <boost/hana/config.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Tag representing a compile-time string.
    //!
    //! Conceptually, a `String` is like a tuple holding `IntegralConstant`s
    //! of type `char`. However, the interface of a `String` is not as rich as
    //! that of a tuple, because a `String` can only hold compile-time
    //! characters as opposed to any kind of object.
    //!
    //! `String`s are used for simple purposes like being keys in a `hana::map`
    //! or tagging the members of a `Struct`. However, you might find that
    //! `String` does not provide enough functionality to be used as a
    //! full-blown compile-time string implementation. Indeed, providing
    //! a comprehensive string interface is a lot of job, and it is out
    //! of the scope of the library for the time being.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! For most purposes, a `String` is functionally equivalent to a tuple
    //! holding `Constant`s of underlying type `char`.
    //!
    //! 1. `Comparable`\n
    //! Two `String`s are equal if and only if they have the same number of
    //! characters and characters at corresponding indices are equal.
    //! @include example/string/comparable.cpp
    //!
    //! 2. `Orderable`\n
    //! The total order implemented for `Orderable` is the usual
    //! lexicographical comparison of strings.
    //! @include example/string/orderable.cpp
    //!
    //! 3. `Foldable`\n
    //! Folding a `String` is equivalent to folding the sequence of its
    //! characters.
    //! @include example/string/foldable.cpp
    //!
    //! 4. `Iterable`\n
    //! Iterating over a `String` is equivalent to iterating over the sequence
    //! of its characters. Also note that `operator[]` can be used instead of
    //! the `at` function.
    //! @include example/string/iterable.cpp
    //!
    //! 5. `Searchable`\n
    //! Searching through a `String` is equivalent to searching through the
    //! sequence of its characters.
    //! @include example/string/searchable.cpp
    //!
    //!
    //! Conversion to `char const*`
    //! ---------------------------
    //! A `String` can be converted to a `constexpr` null-delimited string of
    //! type `char const*` by using `to<char const*>`. This makes it easy to
    //! turn a compile-time string into a runtime string. However, note that
    //! this conversion is not an embedding, because `char const*` does not
    //! model the same concepts as `String` does.
    //! @include example/string/convert.cpp
    //!
    //!
    //! > #### Rationale for `String` not being a `Constant`
    //! > The underlying type held by a `String` could be either `char const*`
    //! > or some other constexpr-enabled string-like container. In the first
    //! > case, `String` can not be a `Constant` because the models of several
    //! > concepts would not be respected by the underlying type, causing
    //! > `value` not to be structure-preserving. Providing an underlying
    //! > value of constexpr-enabled string-like container type like
    //! > `std::string_view` would be great, but that's a bit complicated
    //! > for the time being.
    struct String { };

    //! Create a compile-time string from a parameter pack of characters.
    //! @relates String
    //!
    //!
    //! Example
    //! -------
    //! @include example/string/string.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <char ...s>
    constexpr unspecified-type string{};
#else
    template <char ...s>
    struct string_t;

    template <char ...s>
    constexpr string_t<s...> string{};
#endif

    //! Create a compile-time string from a string literal `s`.
    //! @relates String
    //!
    //! This macro is a more convenient alternative to `string` for creating
    //! compile-time strings. However, since this macro uses a lambda
    //! internally, it can't be used in an unevaluated context.
    //!
    //!
    //! Example
    //! -------
    //! @include example/string/macro.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    auto BOOST_HANA_STRING(s) = see documentation;
    #define BOOST_HANA_STRING(s) see documentation

    // Note:
    // The trick above seems to exploit a bug in Doxygen, which makes the
    // BOOST_HANA_STRING macro appear in the related objects of String
    // (as we want it to).
#else
    // defined in <boost/hana/string.hpp>
#endif

#ifdef BOOST_HANA_CONFIG_ENABLE_STRING_UDL
    namespace literals {
        //! Creates a compile-time `String` from a string literal.
        //! @relatesalso boost::hana::String
        //!
        //! The string literal is parsed at compile-time and the result is
        //! returned as a Hana compile-time `string<...>`. This feature is
        //! an extension that is enabled by default; see below for details.
        //!
        //! @note
        //! Only narrow string literals are supported right now; support for
        //! fancier types of string literals like wide or UTF-XX might be
        //! added in the future if there is a demand for it. See [this issue]
        //! [Hana.issue80] if you need this.
        //!
        //! @warning
        //! This user-defined literal is an extension which requires a special
        //! string literal operator that is not part of the standard yet.
        //! That operator is supported by both Clang and GCC, and several
        //! proposals were made for it to enter C++17. However, since it is
        //! not standard, it is disabled by default and defining the
        //! `BOOST_HANA_CONFIG_ENABLE_STRING_UDL` config macro is required
        //! to get this operator. Hence, if you want to stay safe, just use
        //! the `BOOST_HANA_STRING` macro instead. If you want to be fast and
        //! furious (I do), define `BOOST_HANA_CONFIG_ENABLE_STRING_UDL`.
        //!
        //!
        //! Example
        //! -------
        //! @include example/string/literal.cpp
        //!
        //! [Hana.issue80]: https://github.com/ldionne/hana/issues/80
        template <typename CharT, CharT ...s>
        constexpr auto operator"" _s();
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_STRING_HPP
