/*!
@file
Defines `boost::hana::Enumerable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_ENUMERABLE_HPP
#define BOOST_HANA_CONCEPT_ENUMERABLE_HPP

#include <boost/hana/fwd/concept/enumerable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>

#include <boost/hana/pred.hpp>
#include <boost/hana/succ.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename E>
    struct models_impl<Enumerable, E>
        : _integral_constant<bool,
            !is_default<succ_impl<E>>::value &&
            !is_default<pred_impl<E>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ENUMERABLE_HPP
