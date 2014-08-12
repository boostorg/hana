/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_COMPARABLE_HPP
#define BOOST_HANA_RANGE_COMPARABLE_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/range/range.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Range`s.
    //!
    //! Two ranges are equal if and only if they are both empty or they have
    //! the same `head` and the same length.
    template <>
    struct Comparable::instance<Range, Range> : Comparable::equal_mcd {
        template <typename R1, typename R2>
        static constexpr auto equal_impl(R1 r1, R2 r2) {
            return or_(
                and_(is_empty(r1), is_empty(r2)),
                and_(
                    equal(r1.from, r2.from),
                    equal(r1.to, r2.to)
                )
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_COMPARABLE_HPP
