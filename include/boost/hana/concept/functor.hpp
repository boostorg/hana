/*!
@file
Defines `boost::hana::Functor`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_FUNCTOR_HPP
#define BOOST_HANA_CONCEPT_FUNCTOR_HPP

#include <boost/hana/fwd/concept/functor.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/adjust_if.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/transform.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    template <typename F>
    struct Functor {
        using Tag = typename tag_of<F>::type;
        static constexpr bool value = !is_default<transform_impl<Tag>>::value ||
                                      !is_default<adjust_if_impl<Tag>>::value;
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_FUNCTOR_HPP
