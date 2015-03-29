/*!
@file
Defines `boost::hana::ext::std::Pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_PAIR_HPP
#define BOOST_HANA_EXT_STD_PAIR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/product.hpp>

#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct Pair; }}

    template <typename First, typename Second>
    struct datatype< ::std::pair<First, Second>> {
        using type = ext::std::Pair;
    };

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::std::Pair> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return ::std::make_pair(static_cast<X&&>(x),
                                    static_cast<Y&&>(y));
        }
    };

    template <>
    struct first_impl<ext::std::Pair> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return static_cast<P&&>(p).first; }
    };

    template <>
    struct second_impl<ext::std::Pair> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return static_cast<P&&>(p).second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_PAIR_HPP
