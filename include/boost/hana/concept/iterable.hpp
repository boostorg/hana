/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_ITERABLE_HPP
#define BOOST_HANA_CONCEPT_ITERABLE_HPP

#include <boost/hana/fwd/concept/iterable.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/tail.hpp>


namespace boost { namespace hana {
    template <typename It>
    struct models_impl<Iterable, It> {
        static constexpr bool value = !is_default<at_impl<It>>::value &&
                                      !is_default<tail_impl<It>>::value &&
                                      !is_default<is_empty_impl<It>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ITERABLE_HPP
