/*!
@file
Forward declares `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_MAP_HPP
#define BOOST_HANA_MAP_MAP_HPP

#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic associative array requiring unique and `Comparable` keys.
    //!
    //! The order of the elements of the map is unspecified. Also, all the
    //! keys must be comparable with each other and that comparison must
    //! yield a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! ### Instance of
    //! `Comparable` and `Searchable`
    struct Map { struct hana_enabled_operators : Comparable { }; };

    namespace map_detail {
        template <typename T, typename = operators::enable_adl>
        struct map {
            T storage;
            using hana_datatype = Map;
        };
    }

    //! Creates a `Map` with the given key/value associations.
    //! @relates Map
    //!
    //! @note
    //! The keys must all be unique.
    BOOST_HANA_CONSTEXPR_LAMBDA auto map = [](auto ...pairs) {
        auto storage = tuple(pairs...);
        return map_detail::map<decltype(storage)>{storage};
    };

    //! Returns a list of the keys of the map, in unspecified order.
    //! @relates Map
    BOOST_HANA_CONSTEXPR_LAMBDA auto keys = [](auto map) {
        return fmap(map.storage, first);
    };

    //! Returns a list of the values of the map, in unspecified order.
    //! @relates Map
    BOOST_HANA_CONSTEXPR_LAMBDA auto values = [](auto map) {
        return fmap(map.storage, second);
    };

#if 0
    // insert the given (key, value) pair, or replace the value associated to the key
    BOOST_HANA_CONSTEXPR_LAMBDA auto insert = [](auto key, auto value, auto map) {
        auto add_new = [=](auto _) {
            return detail::wrap<Map>(
                _(cons)(pair(key, value), detail::unwrap(map))
            );
        };
        auto iskey = [=](auto p) { return equal(first(p), key); };
        auto replace_existing = [=](auto _) {
            return detail::wrap<Map>(
                _(replace)(detail::unwrap(map), iskey, pair(key, value))
            );
        };
        return eval_if(elem(keys(map), key), replace_existing, add_new);
    };

    template <>
    struct Foldable::instance<Map> : Foldable::mcd {
        template <typename Map, typename State, typename F>
        static constexpr auto foldr_impl(Map m, State s, F f) {
            return foldr(values(m), s, f);
        }

        template <typename Map, typename State, typename F>
        static constexpr auto foldl_impl(Map m, State s, F f) {
            return foldl(values(m), s, f);
        }

        template <typename Map>
        static constexpr auto length_impl(Map m)
        { return length(detail::unwrap(m)); }
    };

    template <>
    struct Functor::instance<Map> : Functor::fmap_mcd {
        template <typename M, typename F>
        static constexpr auto fmap_impl(M m, F f) {
            auto on_values = [=](auto p) {
                return pair(first(p), f(second(p)));
            };
            return detail::wrap<Map>(fmap(detail::unwrap(m), on_values));
        }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_MAP_HPP
