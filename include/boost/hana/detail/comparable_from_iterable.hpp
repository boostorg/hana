/*!
 * @file
 * Defines `boost::hana::detail::comparable_from_iterable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_COMPARABLE_FROM_ITERABLE_HPP
#define BOOST_HANA_DETAIL_COMPARABLE_FROM_ITERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup details
    struct comparable_from_iterable : defaults<Comparable> {
        template <typename Xs, typename Ys, bool xs_empty, bool ys_empty>
        static constexpr Bool<xs_empty && ys_empty>
        equal_helper(Xs xs, Ys ys, Bool<xs_empty>, Bool<ys_empty>)
        { return {}; }

        template <typename Xs, typename Ys>
        static constexpr auto equal_helper(Xs xs, Ys ys, Bool<false>, Bool<false>)
        { return head(xs) == head(ys) && tail(xs) == tail(ys); }

        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys)
        { return equal_helper(xs, ys, is_empty(xs), is_empty(ys)); }
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_COMPARABLE_FROM_ITERABLE_HPP
