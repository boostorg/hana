/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_COMPARABLE_HPP
#define BOOST_HANA_SET_COMPARABLE_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/searchable.hpp>
#include <boost/hana/set/set.hpp>


namespace boost { namespace hana {
    //! Two sets are equal iff they contain the same elements, regardless of
    //! the order.
    //!
    //! ### Example
    //! example/set.cpp comparable
    template <>
    struct Comparable::instance<Set, Set> : Comparable::equal_mcd {
        template <typename S1, typename S2>
        static constexpr auto equal_impl(S1 s1, S2 s2) {
            return and_(
                equal(length(s1.storage), length(s2.storage)),
                all(s1.storage, [=](auto k) {
                    return equal(lookup(s1, k), lookup(s2, k));
                })
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_COMPARABLE_HPP
