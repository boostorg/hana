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
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/drop_front.hpp>
#include <boost/hana/is_empty.hpp>


namespace boost { namespace hana {
    template <typename It>
    struct Iterable {
        using Tag = typename tag_of<It>::type;
        static constexpr bool value = !is_default<at_impl<Tag>>::value &&
                                      !is_default<drop_front_impl<Tag>>::value &&
                                      !is_default<is_empty_impl<Tag>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ITERABLE_HPP
