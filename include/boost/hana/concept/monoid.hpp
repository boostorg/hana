/*!
@file
Defines `boost::hana::Monoid`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_MONOID_HPP
#define BOOST_HANA_CONCEPT_MONOID_HPP

#include <boost/hana/fwd/concept/monoid.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>

#include <boost/hana/plus.hpp>
#include <boost/hana/zero.hpp>

#include <type_traits>


namespace boost { namespace hana {
    template <typename M>
    struct models_impl<Monoid, M>
        : _integral_constant<bool,
            !is_default<zero_impl<M>>::value &&
            !is_default<plus_impl<M, M>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_MONOID_HPP
