/*!
@file
Defines `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_HPP
#define BOOST_HANA_MAP_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/pair/pair.hpp>
#include <boost/hana/searchable/find_mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! A basic associative array requiring unique and `Comparable` keys.
    //!
    //! The order of the elements of the map is unspecified. Also, all the
    //! keys must be comparable with each other and that comparison must
    //! yield a boolean `Integral`.
    struct Map { };

    //! Creates a `Map` with the given key/value associations.
    //! @relates Map
    //!
    //! @note
    //! The keys must all be unique.
    BOOST_HANA_CONSTEXPR_LAMBDA auto map = [](auto ...pairs) {
        return detail::wrap<Map>(list(pairs...));
    };

    //! Returns a list of the keys of the map, in unspecified order.
    //! @relates Map
    BOOST_HANA_CONSTEXPR_LAMBDA auto keys = [](auto map) {
        return fmap(first, detail::unwrap(map));
    };

    //! Returns a list of the values of the map, in unspecified order.
    //! @relates Map
    BOOST_HANA_CONSTEXPR_LAMBDA auto values = [](auto map) {
        return fmap(second, detail::unwrap(map));
    };

    //! Two maps are equal iff all their keys are equal and are associated
    //! to equal values.
    //!
    //! ### Example
    //! @snippet example/map/comparable.cpp main
    template <>
    struct Comparable::instance<Map, Map> : Comparable::equal_mcd {
        template <typename M1, typename M2>
        static constexpr auto equal_impl(M1 m1, M2 m2) {
            return and_(
                equal(length(detail::unwrap(m1)), length(detail::unwrap(m2))),
                all([=](auto k) {
                    return equal(find_key(k, m1), find_key(k, m2));
                }, keys(m1))
            );
        }
    };

    //! A map can be searched by its keys with a predicate yielding a boolean
    //! `Integral`.
    //!
    //! ### Example
    //! @snippet example/map/searchable.cpp main
    template <>
    struct Searchable::instance<Map> : Searchable::find_mcd {
        template <typename Pred, typename M>
        static constexpr auto find_impl(Pred pred, M map) {
            return fmap(
                second,
                find([=](auto p) { return pred(first(p)); }, detail::unwrap(map))
            );
        }

        template <typename Pred, typename M>
        static constexpr auto any_impl(Pred pred, M map)
        { return any(pred, keys(map)); }
    };

    //! Converts a `List` of `Pair`s to a `Map`.
    //! @relates Map
    //!
    //! @note
    //! The list must not contain duplicate keys.
    template <typename L>
    struct convert<Map, L, detail::enable_if_t<instantiates<List, L>()>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return unpack(map, xs); }
    };

    //! Converts a `Map` to a `List` of `Pair`s.
    //! @relates Map
    template <typename L>
    struct convert<L, Map, detail::enable_if_t<instantiates<List, L>()>> {
        template <typename M>
        static constexpr auto apply(M m)
        { return to<L>(detail::unwrap(m)); }
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
                _(replace)(iskey, pair(key, value), detail::unwrap(map))
            );
        };
        return eval_if(elem(key, keys(map)), replace_existing, add_new);
    };

    template <>
    struct Foldable::instance<Map> : Foldable::mcd {
        template <typename F, typename State, typename Map>
        static constexpr auto foldr_impl(F f, State s, Map m) {
            return foldr(f, s, values(m));
        }

        template <typename F, typename State, typename Map>
        static constexpr auto foldl_impl(F f, State s, Map m) {
            return foldl(f, s, values(m));
        }

        template <typename Map>
        static constexpr auto length_impl(Map m)
        { return length(detail::unwrap(m)); }
    };

    template <>
    struct Functor::instance<Map> : Functor::fmap_mcd {
        template <typename F, typename M>
        static constexpr auto fmap_impl(F f, M m) {
            auto on_values = [=](auto p) {
                return pair(first(p), f(second(p)));
            };
            return detail::wrap<Map>(fmap(on_values, detail::unwrap(m)));
        }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_HPP
