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
        static constexpr auto apply(X x) {
            auto extract = [=](auto k_f) {
                using P = datatype_t<decltype(k_f)>;
                return make_product<P>(first(k_f), second(k_f)(x));
            };
            return to<Map>(fmap(extract, members<R>));
        }
    };

    //! Converts a `List` of `Product`s to a `Map`.
    //! @relates Map
    //!
    //! @note
    //! The list must not contain duplicate keys.
    //!
    //! @todo
    //! - We should actually convert from any `Foldable`
    //! - We should allow duplicate keys, with a documented policy (e.g. we
    //!   keep the last one)
    template <typename L>
    struct convert<Map, L, when<is_a<List, L>()>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return unpack(xs, map); }
    };

    //! Converts a `Map` to a `List` of `Product`s.
    //! @relates Map
    template <typename L>
    struct convert<L, Map, when<is_a<List, L>()>> {
        template <typename M>
        static constexpr auto apply(M m)
        { return to<L>(m.storage); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_CONVERT_HPP
