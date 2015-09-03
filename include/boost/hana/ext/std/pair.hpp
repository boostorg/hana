/*!
@file
Adapts `std::pair` for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_PAIR_HPP
#define BOOST_HANA_EXT_STD_PAIR_HPP

#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/first.hpp>
#include <boost/hana/fwd/second.hpp>

#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct pair_tag; }}

    template <typename First, typename Second>
    struct tag_of<std::pair<First, Second>> {
        using type = ext::std::pair_tag;
    };

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::std::pair_tag> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return std::make_pair(static_cast<X&&>(x),
                                  static_cast<Y&&>(y));
        }
    };

    template <>
    struct first_impl<ext::std::pair_tag> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return static_cast<P&&>(p).first; }
    };

    template <>
    struct second_impl<ext::std::pair_tag> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return static_cast<P&&>(p).second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_PAIR_HPP
