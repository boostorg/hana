/*!
@file
Defines `boost::hana::Orderable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_ORDERABLE_HPP
#define BOOST_HANA_CONCEPT_ORDERABLE_HPP

#include <boost/hana/fwd/concept/orderable.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/less.hpp>


namespace boost { namespace hana {
    template <typename Ord>
    struct models_impl<Orderable, Ord> {
        static constexpr bool value = !is_default<less_impl<Ord, Ord>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ORDERABLE_HPP
