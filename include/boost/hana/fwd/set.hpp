/*!
@file
Forward declares `boost::hana::Set`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SET_HPP
#define BOOST_HANA_FWD_SET_HPP

#include <boost/hana/detail/erase_key_fwd.hpp>
#include <boost/hana/fwd/core/make.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Tag representing a basic unordered container requiring `Comparable`
    //! elements.
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two sets are equal iff they contain the same elements, regardless of
    //! the order.
    //! @snippet example/set.cpp Comparable
    //!
    //! 2. Foldable\n
    //! Folding a `Set` is equivalent to folding the sequence of its values.
    //! However, note that the values are not required to be in any specific
    //! order, so using the folds provided here with an operation that is not
    //! both commutative and associative will yield non-deterministic behavior.
    //! @snippet example/set.cpp Foldable
    //!
    //! 3. Searchable (operators provided)\n
    //! The elements in a `Set` act as both its keys and its values. Since the
    //! elements of a set are unique, searching for an element will return
    //! either the only element which is equal to the searched value, or
    //! `nothing`.
    //! @snippet example/set.cpp Searchable
    //!
    //!
    //! Conversion from any `Foldable`
    //! ------------------------------
    //! Any `Foldable` structure can be converted into a `Set` by using
    //! `to<Set>`. The elements of the structure must all be compile-time
    //! `Comparable`. If the structure contains duplicate elements, only
    //! the first occurence will appear in the resulting set. More
    //! specifically, conversion from a `Foldable` is equivalent to
    //! @code
    //!     to<Set>(xs) == fold_left(xs, make<Set>(), insert)
    //! @endcode
    //!
    //! __Example__
    //! @snippet example/set.cpp from_Foldable
    struct Set { };

    //! Function object for creating a `Set`.
    //! @relates Set
    //!
    //! Given zero or more values `xs...`, `make<Set>` returns a `Set`
    //! containing those values. The values must all be compile-time
    //! `Comparable`, and no duplicate values may be provided. To create
    //! a `Set` from a sequence with possible duplicates, use `to<Set>`
    //! instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/set.cpp make<Set>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <>
    constexpr auto make<Set> = [](auto&& ...xs) {
        return unspecified-type{forwarded(xs)...};
    };
#endif

    template <typename ...Xs>
    struct _set;

    //! Equivalent to `make<Set>`; provided for convenience.
    //! @relates Set
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/set.cpp make_set
    constexpr auto make_set = make<Set>;

    //! Equivalent to `make<Set>`, provided for convenience.
    //! @relates Set
    constexpr auto set = make<Set>;

    //! Insert an element in a `Set`.
    //! @relates Set
    //!
    //! If the set already contains an element that compares equal, then
    //! nothing is done and the set is returned as is.
    //!
    //!
    //! @param set
    //! The set in which to insert a value.
    //!
    //! @param element
    //! The value to insert. It must be compile-time `Comparable`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/set.cpp insert
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto insert = [](auto&& set, auto&& element) -> decltype(auto) {
        return tag-dispatched;
    };
#endif

    //! Remove an element from a `Set`.
    //! @relates Set
    //!
    //! Returns a new `Set` containing all the elements of the original,
    //! except the one comparing `equal` to the given element. If the set
    //! does not contain such an element, a new set equal to the original
    //! set is returned.
    //!
    //!
    //! @param set
    //! The set in which to remove a value.
    //!
    //! @param element
    //! The value to remove. It must be compile-time `Comparable`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/set.cpp erase_key
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto erase_key = [](auto&& set, auto&& element) -> decltype(auto) {
        return tag-dispatched;
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SET_HPP
