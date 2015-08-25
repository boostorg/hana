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

#include <boost/hana/core/default.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct models_impl<Comparable, T> {
        static constexpr bool value = !is_default<equal_impl<T, T>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_COMPARABLE_HPP
