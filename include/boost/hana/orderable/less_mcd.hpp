/*!
@file
Defines `boost::hana::Orderable::less_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_LESS_MCD_HPP
#define BOOST_HANA_ORDERABLE_LESS_MCD_HPP

#include <boost/hana/orderable/orderable.hpp>

#include <boost/hana/detail/logical_fwd.hpp>


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
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_LESS_MCD_HPP

#include <boost/hana/logical.hpp>
