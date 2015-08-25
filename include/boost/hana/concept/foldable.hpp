/*!
@file
Defines `boost::hana::Foldable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_FOLDABLE_HPP
#define BOOST_HANA_CONCEPT_FOLDABLE_HPP

#include <boost/hana/fwd/concept/foldable.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/unpack.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct models_impl<Foldable, T> {
        static constexpr bool value = !is_default<fold_left_impl<T>>::value ||
                                      !is_default<unpack_impl<T>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_FOLDABLE_HPP
