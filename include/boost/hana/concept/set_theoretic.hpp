/*!
@file
Defines `boost::hana::SetTheoretic`.

@copyright Shreyans Doshi 2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_SET_THEORETIC_HPP
#define BOOST_HANA_CONCEPT_SET_THEORETIC_HPP

#include <boost/hana/fwd/concept/set_theoretic.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/detail/integral_constant.hpp>
#include <boost/hana/difference.hpp>
#include <boost/hana/intersection.hpp>
#include <boost/hana/symmetric_difference.hpp>
#include <boost/hana/union.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    template <typename STh>
    struct SetTheoretic
        : hana::integral_constant<bool,
            !is_default<difference_impl<typename tag_of<STh>::type>>::value &&
            !is_default<intersection_impl<typename tag_of<STh>::type>>::value &&
            !is_default<union_impl<typename tag_of<STh>::type>>::value
        >
    { };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_SET_THEORETIC_HPP
