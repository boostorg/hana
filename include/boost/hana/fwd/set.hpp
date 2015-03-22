/*!
@file
Forward declares `boost::hana::Set`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SET_HPP
#define BOOST_HANA_FWD_SET_HPP

#include <boost/hana/fwd/core/make.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic unordered container requiring `Comparable` elements.
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two sets are equal iff they contain the same elements, regardless of
    //! the order.
    //! @snippet example/set.cpp comparable
    //!
    //! 2. Foldable\n
    //! Folding a `Set` is equivalent to folding the sequence of its values.
    //! However, note that the values are not required to be in any specific
    //! order, so using the folds provided here with an operation that is not
    //! both commutative and associative will yield unspecified behavior.
    //! @snippet example/set.cpp foldable
    //!
    //! 3. Searchable\n
    //! The keys and the values of a `Set` are its elements; the `Searchable` 
    //! model follows naturally from that.
    //! @snippet example/set.cpp searchable
    //!
    //!
    //! Provided conversions
    //! --------------------
    //! 1. From any `Foldable`\n
    //! If the foldable structure contains duplicates, the last one will
    //! be the one appearing in the resulting set.
    struct Set { };

    //! Creates a `Set` containing the given elements.
    //! @relates Set
    //!
    //! @note
    //! There must not be duplicate elements.
    //!
    //! @todo Consider allowing duplicates elements in this constructor.
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
    //! @param x
    //! The value to insert.
    //!
    //!
    //! ### Example
    //! @snippet example/set.cpp insert
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto insert = [](auto&& set, auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SET_HPP
