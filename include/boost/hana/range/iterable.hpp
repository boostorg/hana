/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_ITERABLE_HPP
#define BOOST_HANA_RANGE_ITERABLE_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/orderable/orderable.hpp>
#include <boost/hana/range/range.hpp>


namespace boost { namespace hana {
    //! Instance of `Iterable` for `Range`s.
    //!
    //! Let `r` be a `Range` containing the `Integral`s in the half-open
    //! interval `[from, to)`. The head of `r` is an `Integral` with value
    //! `from`, its tail is the range representing the `[from + 1, to)`
    //! interval and `r` is empty if and only if `from == to`.
    //!
    //! @snippet example/range.cpp iterable
    //!
    //! @todo
    //! Consider allowing the elements of the range to be any `Group`, since
    //! we don't use much more than that (except `Comparable` and `Orderable`).
    template <>
    struct Iterable::instance<Range> : Iterable::mcd {
        template <typename R>
        static constexpr auto head_impl(R r)
        { return r.from; }

        //! @todo
        //! We need a type class to express the concept of
        //! incrementing and decrementing.
        template <typename R>
        static constexpr auto tail_impl(R r)
        { return range(plus(r.from, int_<1>), r.to); }

        template <typename R>
        static constexpr auto is_empty_impl(R r)
        { return equal(r.from, r.to); }

        template <typename N, typename R>
        static constexpr auto at_impl(N n, R r)
        { return plus(r.from, n); }

        template <typename R>
        static constexpr auto last_impl(R r)
        { return minus(r.to, int_<1>); }

        template <typename N, typename R>
        static constexpr auto drop_impl(N n, R r) {
            auto size = minus(r.to, r.from);
            return range(if_(greater(n, size), r.to, plus(r.from, n)), r.to);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_ITERABLE_HPP
