/*!
@file
Defines conversions to and from a `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_CONVERT_HPP
#define BOOST_HANA_MAP_CONVERT_HPP

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/map/map.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/record/record.hpp>


namespace boost { namespace hana {
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_CONVERT_HPP
