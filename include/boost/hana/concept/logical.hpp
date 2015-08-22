/*!
@file
Defines `boost::hana::Logical`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_LOGICAL_HPP
#define BOOST_HANA_CONCEPT_LOGICAL_HPP

#include <boost/hana/fwd/concept/logical.hpp>

//! @todo Get rid of these includes
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/lazy.hpp>

#include <type_traits>
#include <utility>

#include <boost/hana/not.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/while.hpp>
#include <boost/hana/and.hpp>
#include <boost/hana/or.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename L>
    struct models_impl<Logical, L>
        : _integral_constant<bool,
            !is_default<eval_if_impl<L>>::value &&
            !is_default<not_impl<L>>::value &&
            !is_default<while_impl<L>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_LOGICAL_HPP
