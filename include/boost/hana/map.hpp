/*!
@file
Defines `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_HPP
#define BOOST_HANA_MAP_HPP

#include <boost/hana/fwd/map.hpp>

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/record.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //! Two maps are equal iff all their keys are equal and are associated
    //! to equal values.
    //!
    //! ### Example
    //! @snippet example/map.cpp comparable
    template <>
    struct equal_impl<Map, Map> {
        template <typename M1, typename M2>
        static constexpr auto apply(M1 m1, M2 m2) {
            return and_(
                equal(length(m1.storage), length(m2.storage)),
                all(keys(m1), [=](auto k) {
                    return equal(lookup(m1, k), lookup(m2, k));
                })
            );
        }
    };

    //! Converting a `Record` `R` to a `Map` is equivalent to converting its
    //! `members<R>` to a `Map`, except the values are replaced by the actual
    //! members of the object instead of accessors.
    template <typename R>
    struct convert<Map, R, when<is_a<Record, R>()>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x) {
            auto extract = [x(detail::std::forward<X>(x))](auto&& member) -> decltype(auto) {
                using P = datatype_t<decltype(member)>;
                return make<P>(
                    first(detail::std::forward<decltype(member)>(member)),
                    second(detail::std::forward<decltype(member)>(member))(x)
                );
            };
            return to<Map>(fmap(members<R>, detail::std::move(extract)));
        }
    };

    //! Converts a `Foldable` of `Product`s to a `Map`.
    //! @relates Map
    //!
    //! @note
    //! The foldable structure must not contain duplicate keys.
    //!
    //! @todo
    //! We should allow duplicate keys, with a documented policy (e.g. we
    //! keep the last one).
    template <typename F>
    struct convert<Map, F, when<is_a<Foldable, F>() && !is_a<Record, F>()>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return unpack(detail::std::forward<Xs>(xs), map); }
    };

    //! Converts a `Map` to a `List` of `Product`s.
    //! @relates Map
    template <typename L>
    struct convert<L, Map, when<is_a<List, L>()>> {
        template <typename M>
        static constexpr decltype(auto) apply(M&& m)
        { return to<L>(detail::std::forward<M>(m).storage); }
    };

    //! A map can be searched by its keys with a predicate yielding a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! ### Example
    //! @snippet example/map.cpp searchable
    template <>
    struct find_impl<Map> {
        template <typename M, typename Pred>
        static constexpr auto apply(M map, Pred pred) {
            return fmap(
                find(map.storage, [=](auto p) {
                    return pred(first(p));
                }),
                second
            );
        }
    };

    template <>
    struct any_impl<Map> {
        template <typename M, typename Pred>
        static constexpr auto apply(M map, Pred pred)
        { return any(keys(map), pred); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_HPP
