/*!
@file
Forward declares `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SET_HPP
#define BOOST_HANA_FWD_SET_HPP

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>


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
    //! 2. Searchable\n
    //! The keys and the values of a `Set` are its elements; the `Searchable` 
    //! instance follows naturally from that.
    //! @snippet example/set.cpp searchable
    //!
    //!
    //! Provided conversions
    //! --------------------
    //! 1. To any `Sequence`\n
    //! The order of the elements in the resulting sequence is unspecified.
    //!
    //! 2. From any `Foldable`\n
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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto set = [](auto&& ...xs) {
        return unspecified-type;
    };
#else
    template <typename Storage, typename = operators::adl>
    struct _set {
        Storage storage;
        struct hana { using datatype = Set; };
    };

    struct _make_set {
        template <typename ...Xs>
        constexpr decltype(auto) operator()(Xs&& ...xs) const;
    };

    constexpr _make_set set{};
#endif

    //! Equivalent to `set`, provided for consistency.
    //! @relates Set
    template <>
    struct make_impl<Set> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return hana::set(detail::std::forward<Xs>(xs)...);
        }
    };

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
