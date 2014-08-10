/*!
@file
Adapts `std::pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_PAIR_HPP
#define BOOST_HANA_EXT_STD_PAIR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/product/mcd.hpp>

#include <utility>


namespace boost { namespace hana {
    struct StdPair;

    template <typename First, typename Second>
    struct datatype<std::pair<First, Second>> {
        using type = StdPair;
    };

    template <>
    struct Product::instance<StdPair> : Product::mcd {
        template <typename X, typename Y>
        static auto make_product_impl(X x, Y y)
        { return std::make_pair(x, y); }

        template <typename P>
        static auto first_impl(P p)
        { return p.first; }

        template <typename P>
        static auto second_impl(P p)
        { return p.second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_PAIR_HPP
