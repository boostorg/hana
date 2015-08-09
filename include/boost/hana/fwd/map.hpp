/*!
@file
Forward declares `boost::hana::Map`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MAP_HPP
#define BOOST_HANA_FWD_MAP_HPP

#include <boost/hana/fwd/keys.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/erase_key.hpp>
#include <boost/hana/fwd/insert.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Tag representing a basic associative array requiring unique and
    //! `Comparable` keys.
    //!
    //! The order of the elements of the map is unspecified. Also, all the
    //! keys must be comparable with each other and that comparison must
    //! yield a compile-time `Logical`.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable`\n
    //! Two maps are equal iff all their keys are equal and are associated
    //! to equal values.
    //! @include example/map/comparable.cpp
    //!
    //! 2. `Searchable`\n
    //! A map can be searched by its keys with a predicate yielding a
    //! compile-time `Logical`. Also note that `operator[]` can be used
    //! instead of `at_key`.
    //! @include example/map/searchable.cpp
    //!
    //! 3. `Foldable`\n
    //! Folding a map is equivalent to folding a list of the key/value pairs
    //! it contains. In particular, since that list is not guaranteed to be
    //! in any specific order, folding a map with an operation that is not
    //! both commutative and associative will yield non-deterministic behavior.
    //! @include example/map/foldable.cpp
    //!
    //!
    //! Provided constructions
    //! ----------------------
    //! 1. From any `Foldable`\n
    //! Converts a `Foldable` of `Product`s to a `Map`. If the structure
    //! contains duplicate keys, only the value associated to the first
    //! occurence of each key is kept.
    //! @include example/map/convert.cpp
    struct Map { };

    template <typename ...Pairs>
    struct map;

    //! Function object for creating a `Map`.
    //! @relates Map
    //!
    //! Given zero or more`Product`s representing key/value associations,
    //! `make<Map>` returns a `Map` associating these keys to these values.
    //! All the keys must be unique.
    //!
    //!
    //! Example
    //! -------
    //! @include example/map/make.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
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
    //! @include example/map/make.cpp
    constexpr auto make_map = make<Map>;

    //! Returns a Sequence of the keys of the map, in unspecified order.
    //! @relates Map
    //!
    //!
    //! Example
    //! -------
    //! @include example/map/keys.cpp
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
    //! @include example/map/values.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto values = [](auto&& map) -> decltype(auto) {
        return unspecified-type;
    };
#else
    struct values_t {
        template <typename Map>
        constexpr decltype(auto) operator()(Map&& map) const;
    };

    constexpr values_t values{};
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
    //! in the map. The `key` must be compile-time `Comparable`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/map/insert.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto insert = [](auto&& map, auto&& pair) -> decltype(auto) {
        return tag-dispatched;
    };
#endif

    //! Removes a key/value pair from a `Map`.
    //! @relates Map
    //!
    //! Returns a new `Map` containing all the elements of the original,
    //! except for the `(key, value)` pair whose `key` compares `equal`
    //! to the given key. If the `Map` does not contain such an element,
    //! a new `Map` equal to the original is returned.
    //!
    //!
    //! @param map
    //! The map in which to erase a `key`.
    //!
    //! @param key
    //! A key to remove from the `Map`. It must be compile-time `Comparable`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/map/erase_key.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto erase_key = [](auto&& map, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MAP_HPP
