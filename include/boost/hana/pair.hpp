/*!
@file
Defines `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/fwd/pair.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/product.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Pair>
        : operators::of<Comparable, Orderable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Pair> {
        template <typename F, typename S>
        static constexpr decltype(auto) apply(F&& f, S&& s) {
            return hana::pair(detail::std::forward<F>(f),
                              detail::std::forward<S>(s));
        }
    };

    template <>
    struct first_impl<Pair> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return detail::std::forward<P>(p).first; }
    };

    template <>
    struct second_impl<Pair> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return detail::std::forward<P>(p).second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
