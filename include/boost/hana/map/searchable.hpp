/*!
@file
Defines the instance of `boost::hana::Searchable` for `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_SEARCHABLE_HPP
#define BOOST_HANA_MAP_SEARCHABLE_HPP

#include <boost/hana/functor/functor.hpp>
#include <boost/hana/map/map.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/searchable/mcd.hpp>


namespace boost { namespace hana {
    //! A map can be searched by its keys with a predicate yielding a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! ### Example
    //! @snippet example/map.cpp searchable
    template <>
    struct Searchable::instance<Map> : Searchable::mcd {
        template <typename M, typename Pred>
        static constexpr auto find_impl(M map, Pred pred) {
            return fmap(
                second,
                find(map.storage, [=](auto p) {
                    return pred(first(p));
                })
            );
        }

        template <typename M, typename Pred>
        static constexpr auto any_impl(M map, Pred pred)
        { return any(keys(map), pred); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_SEARCHABLE_HPP
