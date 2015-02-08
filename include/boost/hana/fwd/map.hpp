/*!
@file
Forward declares `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MAP_HPP
#define BOOST_HANA_FWD_MAP_HPP

#include <boost/hana/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic associative array requiring unique and `Comparable` keys.
    //!
    //! The order of the elements of the map is unspecified. Also, all the
    //! keys must be comparable with each other and that comparison must
    //! yield a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two maps are equal iff all their keys are equal and are associated
    //! to equal values.
    //! @snippet example/map.cpp comparable
    //!
    //! 2. `Searchable`\n
    //! A map can be searched by its keys with a predicate yielding a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //! @snippet example/map.cpp searchable
    //!
    //!
    //! Provided conversions
    //! --------------------
    //! 1. From any `Record`\n
    //! Converting a `Record` `R` to a `Map` is equivalent to converting its
    //! `members<R>()` to a `Map`, except the values are replaced by the actual
    //! members of the object instead of accessors.
    //!
    //! 2. From any `Foldable`\n
    //! Converts a `Foldable` of `Product`s to a `Map`.
    //! Note that the foldable structure must not contain duplicate keys.
    //! @todo
    //! We should allow duplicate keys, with a documented policy (e.g. we
    //! keep the last one).
    //!
    //! 3. To any `List` data type\n
    //! A `Map` can be converted to a `List` of `Product`s.
    struct Map { };

    //! Creates a `Map` with the given key/value associations.
    //! @relates Map
    //!
    //! @note
    //! The keys must all be unique.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto map = [](auto&& ...pairs) {
        return unspecified-type;
    };
#else
    template <typename Storage, typename = operators::enable_adl>
    struct _map {
        Storage storage;
        struct hana { using datatype = Map; };
    };

    struct _make_map {
        template <typename ...Pairs>
        constexpr decltype(auto) operator()(Pairs&& ...pairs) const;
    };

    constexpr _make_map map{};
#endif

    //! Returns a list of the keys of the map, in unspecified order.
    //! @relates Map
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto keys = [](auto&& map) -> decltype(auto) {
        return unspecified-type;
    };
#else
    struct _keys {
        template <typename Map>
        constexpr decltype(auto) operator()(Map&& map) const;
    };

    constexpr _keys keys{};
#endif

    //! Returns a list of the values of the map, in unspecified order.
    //! @relates Map
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MAP_HPP
