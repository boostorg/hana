/*!
@file
Forward declares `boost::hana::Orderable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_ORDERABLE_HPP
#define BOOST_HANA_ORDERABLE_ORDERABLE_HPP

#include <boost/hana/orderable/detail/orderable_fwd.hpp>

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `less`
    struct Orderable::less_mcd {
        template <typename X, typename Y>
        static constexpr auto less_equal_impl(X x, Y y)
        { return not_(less(y, x)); }

        template <typename X, typename Y>
        static constexpr auto greater_impl(X x, Y y)
        { return less(y, x); }

        template <typename X, typename Y>
        static constexpr auto greater_equal_impl(X x, Y y)
        { return not_(less(x, y)); }

        template <typename X, typename Y>
        static constexpr auto min_impl(X x, Y y)
        { return if_(less(x, y), x, y); }

        template <typename X, typename Y>
        static constexpr auto max_impl(X x, Y y)
        { return if_(less(x, y), y, x); }
    };

    //! Instance of `Orderable` for objects with `LessThanComparable` data
    //! types.
    //!
    //! Any two objects whose data types can be compared using `operator<`
    //! are automatically instances of `Orderable` by using that comparison.
    template <typename X, typename Y>
    struct Orderable::instance<X, Y, when<is_valid<decltype((void)(*(X*)0 < *(Y*)0))>>>
        : Orderable::less_mcd
    {
        static constexpr auto less_impl(X x, Y y)
        { return x < y; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_ORDERABLE_HPP
