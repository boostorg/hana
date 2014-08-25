/*!
@file
Defines `boost::hana::Product::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PRODUCT_MCD_HPP
#define BOOST_HANA_PRODUCT_MCD_HPP

#include <boost/hana/product/product.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `first`, `second` and `make`.
    struct Product::mcd { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRODUCT_MCD_HPP
