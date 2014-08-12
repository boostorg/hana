/*!
@file
Defines `boost::hana::Logical::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_MCD_HPP
#define BOOST_HANA_LOGICAL_MCD_HPP

#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `eval_if` and `not_`
    struct Logical::mcd {
        template <typename X, typename Y>
        static constexpr auto or_impl(X x, Y y)
        { return if_(x, x, y); }

        template <typename X, typename Y>
        static constexpr auto and_impl(X x, Y y)
        { return if_(x, y, x); }

        template <typename C, typename T, typename E>
        static constexpr auto if_impl(C c, T t, E e) {
            return eval_if(c,
                [=](auto) { return t; },
                [=](auto) { return e; }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_MCD_HPP
