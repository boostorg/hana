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

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>

#include <boost/hana/quot.hpp>
#include <boost/hana/rem.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename D>
    struct models_impl<IntegralDomain, D>
        : _integral_constant<bool,
            !is_default<rem_impl<D, D>>::value &&
            !is_default<quot_impl<D, D>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_INTEGRAL_DOMAIN_HPP
