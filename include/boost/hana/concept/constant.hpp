/*!
@file
Defines `boost::hana::Constant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_CONSTANT_HPP
#define BOOST_HANA_CONCEPT_CONSTANT_HPP

#include <boost/hana/fwd/concept/constant.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/value.hpp>


namespace boost { namespace hana {
    template <typename C>
    struct models_impl<Constant, C> {
        static constexpr bool value = !is_default<value_impl<C>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_CONSTANT_HPP
