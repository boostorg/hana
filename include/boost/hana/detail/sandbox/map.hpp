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
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/foldable/lazy_foldr_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/pair/instance.hpp>


namespace boost { namespace hana {
    struct Map;

    BOOST_HANA_CONSTEXPR_LAMBDA auto map = [](auto ...pairs) {
        return detail::wrap<Map>(list(pairs...));
    };

    // a list of the keys
    BOOST_HANA_CONSTEXPR_LAMBDA auto keys = [](auto map) {
        return fmap(first, detail::unwrap(map));
    };

    // a list of the values
    BOOST_HANA_CONSTEXPR_LAMBDA auto values = [](auto map) {
        return fmap(second, detail::unwrap(map));
    };

    // a Maybe value
    BOOST_HANA_CONSTEXPR_LAMBDA auto lookup = [](auto key, auto map) {
        auto equal_key = [=](auto p) { return equal(key, first(p)); };
        return fmap(second, find(equal_key, detail::unwrap(map)));
    };

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
    struct Foldable::instance<Map> : Foldable::lazy_foldr_mcd {
        template <typename F, typename State, typename Map>
        static constexpr auto lazy_foldr_impl(F f, State s, Map m) {
            return lazy_foldr(f, s, values(m));
        }

        template <typename Map>
        static constexpr auto length_impl(Map m)
        { return length(detail::unwrap(m)); }
    };

    template <>
    struct Comparable::instance<Map, Map> : Comparable::equal_mcd {
        template <typename M1, typename M2>
        static constexpr auto equal_impl(M1 m1, M2 m2) {
            return and_(
                equal(length(m1), length(m2)),
                all([=](auto k) {
                    return equal(lookup(k, m1), lookup(k, m2));
                }, keys(m1))
            );
        }
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_HPP
