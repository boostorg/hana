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

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/pred.hpp>
#include <boost/hana/succ.hpp>


namespace boost { namespace hana {
    template <typename E>
    struct Enumerable {
        using Tag = typename tag_of<E>::type;
        static constexpr bool value = !is_default<succ_impl<Tag>>::value &&
                                      !is_default<pred_impl<Tag>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ENUMERABLE_HPP
