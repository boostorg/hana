/*!
@file
Defines the instance of `boost::hana::Product` for `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_PRODUCT_HPP
#define BOOST_HANA_PAIR_PRODUCT_HPP

#include <boost/hana/pair/pair.hpp>
#include <boost/hana/product/mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Product` for `Pair`s.
    //!
    //! ### Example
    //! @snippet example/pair.cpp product
    template <>
    struct Product::instance<Pair> : Product::mcd {
        template <typename X, typename Y>
        static constexpr auto make_product_impl(X x, Y y)
        { return pair(x, y); }

        template <typename P>
        static constexpr auto first_impl(P p)
        { return p.first; }

        template <typename P>
        static constexpr auto second_impl(P p)
        { return p.second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_PRODUCT_HPP
