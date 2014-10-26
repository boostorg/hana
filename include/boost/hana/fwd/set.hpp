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
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic unordered container requiring `Comparable` elements.
    struct Set { struct hana { struct enabled_operators : Comparable { }; }; };

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
    template <typename Storage, typename = operators::enable_adl>
    struct _set {
        Storage storage;
        struct hana { using datatype = Set; };
    };

    struct _make_set {
        template <typename ...Xs>
        constexpr decltype(auto) operator()(Xs&& ...xs) const {
            return detail::create<_set>{}(tuple(detail::std::forward<Xs>(xs)...));
        }
    };

    constexpr _make_set set{};
#endif

    //! Equivalent to `set`, provided for consistency.
    //! @relates Set
    template <>
    constexpr auto make<Set> = set;

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
        return unspecified;
    };
#endif

#if 0
    template <>
    struct Foldable::instance<Set> : Foldable::mcd {
        template <typename Set, typename State, typename F>
        static constexpr auto foldr_impl(Set set, State s, F f) {
            return foldr(detail::unwrap(set), s, f);
        }

        template <typename Set, typename State, typename F>
        static constexpr auto foldl_impl(Set set, State s, F f) {
            return foldl(detail::unwrap(set), s, f);
        }

        template <typename Set>
        static constexpr auto length_impl(Set set)
        { return length(detail::unwrap(set)); }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SET_HPP
