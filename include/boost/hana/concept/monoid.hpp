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

#include <boost/hana/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/zero.hpp>


namespace boost { namespace hana {
    template <typename M>
    struct models_impl<Monoid, M> {
        static constexpr bool value = !is_default<zero_impl<M>>::value &&
                                      !is_default<plus_impl<M, M>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_MONOID_HPP
