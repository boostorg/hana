/*!
@file
Defines `boost::hana::Product`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_PRODUCT_HPP
#define BOOST_HANA_CONCEPT_PRODUCT_HPP

#include <boost/hana/fwd/concept/product.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>


namespace boost { namespace hana {
    template <typename P>
    struct Product {
        using Tag = typename tag_of<P>::type;
        static constexpr bool value = !is_default<first_impl<Tag>>::value &&
                                      !is_default<second_impl<Tag>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_PRODUCT_HPP
