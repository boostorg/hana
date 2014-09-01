/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_COMPARABLE_HPP
#define BOOST_HANA_MAP_COMPARABLE_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/map/map.hpp>
#include <boost/hana/map/searchable.hpp>


namespace boost { namespace hana {
    //! Two maps are equal iff all their keys are equal and are associated
    //! to equal values.
    //!
    //! ### Example
    //! @snippet example/map.cpp comparable
    template <>
    struct Comparable::instance<Map, Map> : Comparable::equal_mcd {
        template <typename M1, typename M2>
        static constexpr auto equal_impl(M1 m1, M2 m2) {
            return and_(
                equal(length(m1.storage), length(m2.storage)),
                all(keys(m1), [=](auto k) {
                    return equal(lookup(m1, k), lookup(m2, k));
                })
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_COMPARABLE_HPP
