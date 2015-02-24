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
    //! Represents a compile-time string.
    //!
    //! Conceptually, a `String` is like a `Tuple` holding `IntegralConstant`s
    //! of type `char`. However, the interface of a `String` is not as rich as
    //! that of a `Tuple`, because a `String` can only hold objects of a
    //! single generalized type.
    //!
    //! `String`s are used for simple purposes like being keys in a `Map` or
    //! tagging the members of a `Struct`. However, you might find that
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
    //! 1. `Comparable` (operators provided)\n
    //! Two `String`s are equal if and only if they have the same number of
    //! characters and characters at corresponding indices are equal.
    //! @snippet example/string.cpp Comparable
    //!
    //! 2. `Orderable` (operators provided)\n
    //! The total order implemented for `Orderable` is the usual
    //! lexicographical comparison of strings.
    //! @snippet example/string.cpp Orderable
    //!
    //! 3. `Foldable`\n
    //! Folding a `String` is equivalent to folding the sequence of its
    //! characters.
    //! @snippet example/string.cpp Foldable
    //!
    //! 4. `Iterable` (operators provided)\n
    //! Iterating over a `String` is equivalent to iterating over the sequence
    //! of its characters.
    //! @snippet example/string.cpp Iterable
    //!
    //! 5. `Searchable`\n
    //! Searching through a `String` is equivalent to searching through the
    //! sequence of its characters.
    //! @snippet example/string.cpp Searchable
    //!
    //!
    //! Conversion to `char const*`
    //! ---------------------------
    //! A `String` can be converted to a `constexpr` null-delimited string of
    //! type `char const*` by using `to<char const*>`. This makes it easy to
    //! turn a compile-time string into a runtime string. However, note that
    //! this conversion is not an embedding, because `char const*` does not
    //! model the same concepts as `String` does.
    //! @snippet example/string.cpp to<char const*>
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
    //! @snippet example/string.cpp string
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <char ...s>
    constexpr unspecified-type string{};
#else
    template <char ...s>
    struct _string;

    template <char ...s>
    constexpr _string<s...> string{};
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
    //! @snippet example/string.cpp BOOST_HANA_STRING
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    auto BOOST_HANA_STRING(s) = see documentation;
    #define BOOST_HANA_STRING(s) see documentation

    // Note:
    // The trick above seems to exploit a bug in Doxygen, which makes the
    // BOOST_HANA_STRING macro appear in the related objects of String
    // (as we want it to).
#else
    // defined in boost/hana/string.hpp
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_STRING_HPP
