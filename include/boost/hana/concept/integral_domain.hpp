/*!
@file
Defines `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_CONCEPT_INTEGRAL_DOMAIN_HPP

#include <boost/hana/fwd/concept/integral_domain.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/div.hpp>
#include <boost/hana/mod.hpp>


namespace boost { namespace hana {
    template <typename D>
    struct IntegralDomain {
        using Tag = typename tag_of<D>::type;
        static constexpr bool value = !is_default<mod_impl<Tag, Tag>>::value &&
                                      !is_default<div_impl<Tag, Tag>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_INTEGRAL_DOMAIN_HPP
