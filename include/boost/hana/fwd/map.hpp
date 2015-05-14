/*!
@file
Forward declares `boost::hana::Map`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MAP_HPP
#define BOOST_HANA_FWD_MAP_HPP

#include <boost/hana/detail/insert_fwd.hpp>
#include <boost/hana/detail/keys_fwd.hpp>
#include <boost/hana/fwd/core/make.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic associative array requiring unique and `Comparable` keys.
    //!
    //! The order of the elements of the map is unspecified. Also, all the
    //! keys must be comparable with each other and that comparison must
    //! yield a compile-time `Logical`.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two maps are equal iff all their keys are equal and are associated
    //! to equal values.
    //! @snippet example/map.cpp Comparable
    //!
    //! 2. `Searchable` (operators provided)\n
    //! A map can be searched by its keys with a predicate yielding a
    //! compile-time `Logical`.
    //! @snippet example/map.cpp Searchable
    //!
    //! 3. `Foldable`\n
    //! Folding a map is equivalent to folding a list of the key/value pairs
    //! it contains. In particular, since that list is not guaranteed to be
    //! in any specific order, folding a map with an operation that is not
    //! both commutative and associative will yield non-deterministic behavior.
    //! @snippet example/map.cpp Foldable
    //!
    //!
    //! Provided constructions
    //! ----------------------
    //! 1. From any `Foldable`\n
    //! Converts a `Foldable` of `Product`s to a `Map`. If the structure
    //! contains duplicate keys, only the value associated to the first
    //! occurence of each key is kept.
    //! @snippet example/map.cpp from_Foldable
    struct Map { };

    template <typename ...Pairs>
    struct _map;

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Create a `Map` with the given key/value associations.
    //! @relates Map
    //!
    //! Given zero `Product`s or more representing key/value associations,
    //! `make<Map>` returns a Map associating these keys to these values.
    //! All the keys must be unique.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/map.cpp make<Map>
    template <>
    constexpr auto make<Map> = [](auto&& ...pairs) {
        return unspecified-type{forwarded(pairs)...};
    };
#endif

    //! Alias to `make<Map>`; provided for convenience.
    //! @relates Map
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/map.cpp make_map
    constexpr auto make_map = make<Map>;

    //! Returns a Sequence of the keys of the map, in unspecified order.
    //! @relates Map
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/map.cpp keys
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto keys = [](auto&& map) -> decltype(auto) {
        return unspecified-type;
    };
#endif

    //! Returns a Sequence of the values of the map, in unspecified order.
    //! @relates Map
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/map.cpp values
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto values = [](auto&& map) -> decltype(auto) {
        return unspecified-type;
    };
#else
    struct _values {
        template <typename Map>
        constexpr decltype(auto) operator()(Map&& map) const;
    };

    constexpr _values values{};
#endif

    //! Inserts a new key/value pair in a `Map`.
    //! @relates Map
    //!
    //! Given a `(key, value)` pair, `insert` inserts this new pair into a
    //! map. If the map already contains this key, nothing is done and the
    //! map is returned as-is.
    //!
    //!
    //! @param map
    //! The map in which to insert a `(key,value)` pair.
    //!
    //! @param pair
    //! An arbitrary `Product` representing a `(key, value)` pair to insert
    //! in the map.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/map.cpp insert
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto insert = [](auto&& map, auto&& pair) -> decltype(auto) {
        return tag-dispatched;
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MAP_HPP
