/*!
@file
Defines `boost::hana::Logical`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_LOGICAL_HPP
#define BOOST_HANA_CONCEPT_LOGICAL_HPP

#include <boost/hana/fwd/concept/logical.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/while.hpp>


namespace boost { namespace hana {
    template <typename L>
    struct Logical {
        using Tag = typename tag_of<L>::type;
        static constexpr bool value = !is_default<eval_if_impl<Tag>>::value &&
                                      !is_default<not_impl<Tag>>::value &&
                                      !is_default<while_impl<Tag>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_LOGICAL_HPP
