/*!
@file
Defines `boost::hana::Hashable`.

@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_HASHABLE_HPP
#define BOOST_HANA_CONCEPT_HASHABLE_HPP

#include <boost/hana/fwd/concept/hashable.hpp>

#include <boost/hana/config.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    template <typename X>
    struct Hashable {
        static constexpr bool value = true;
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_HASHABLE_HPP
