/*!
@file
Defines `boost::hana::Group`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_GROUP_HPP
#define BOOST_HANA_CONCEPT_GROUP_HPP

#include <boost/hana/fwd/concept/group.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/wrong.hpp>
#include <boost/hana/concept/monoid.hpp>

#include <boost/hana/minus.hpp>
#include <boost/hana/negate.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename G>
    struct models_impl<Group, G>
        : _integral_constant<bool,
            !is_default<negate_impl<G>>::value ||
            !is_default<minus_impl<G, G>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_GROUP_HPP
