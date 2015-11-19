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

#include <boost/hana/config.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/unpack.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    template <typename T>
    struct Foldable {
        using Tag = typename tag_of<T>::type;
        static constexpr bool value = !is_default<fold_left_impl<Tag>>::value ||
                                      !is_default<unpack_impl<Tag>>::value;
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_FOLDABLE_HPP
