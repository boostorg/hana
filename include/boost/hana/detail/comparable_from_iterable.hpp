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
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup details
    struct comparable_from_iterable : defaults<Comparable> {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys) {
            return if_(is_empty(xs) || is_empty(ys),
                [](auto xs, auto ys) {
                    return is_empty(xs) && is_empty(ys);
                },
                [](auto xs, auto ys) {
                    return equal(head(xs), head(ys)) &&
                           equal_impl(tail(xs), tail(ys));
                }
            )(xs, ys);
        }
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_COMPARABLE_FROM_ITERABLE_HPP
