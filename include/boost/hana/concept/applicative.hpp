/*!
@file
Defines `boost::hana::Applicative`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_APPLICATIVE_HPP
#define BOOST_HANA_CONCEPT_APPLICATIVE_HPP

#include <boost/hana/fwd/concept/applicative.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>

#include <boost/hana/ap.hpp>
#include <boost/hana/lift.hpp>


namespace boost { namespace hana {
    template <typename A>
    struct models_impl<Applicative, A> {
        static constexpr bool value = !is_default<ap_impl<A>>::value &&
                                      !is_default<lift_impl<A>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_APPLICATIVE_HPP
