/*!
@file
Defines `boost::hana::Comparable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_COMPARABLE_HPP
#define BOOST_HANA_CONCEPT_COMPARABLE_HPP

#include <boost/hana/fwd/concept/comparable.hpp>

//! @todo Get rid of these includes
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/concepts.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/concept/logical.hpp>

#include <boost/hana/comparing.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct models_impl<Comparable, T>
        : _integral_constant<bool,
            !is_default<equal_impl<T, T>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_COMPARABLE_HPP
