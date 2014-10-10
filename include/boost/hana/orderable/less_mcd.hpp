/*!
@file
Defines `boost::hana::Orderable::less_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_LESS_MCD_HPP
#define BOOST_HANA_ORDERABLE_LESS_MCD_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/orderable/orderable.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `less`
    struct Orderable::less_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) less_equal_impl(X&& x, Y&& y) {
            return not_(less(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            ));
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) greater_impl(X&& x, Y&& y) {
            return less(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            );
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) greater_equal_impl(X x, Y y) {
            return not_(less(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            ));
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) min_impl(X&& x, Y&& y) {
            decltype(auto) cond = less(x, y);
            return if_(detail::std::forward<decltype(cond)>(cond),
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) max_impl(X&& x, Y&& y) {
            decltype(auto) cond = less(x, y);
            return if_(detail::std::forward<decltype(cond)>(cond),
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            );
        }
    };
}}

#endif // !BOOST_HANA_ORDERABLE_LESS_MCD_HPP
