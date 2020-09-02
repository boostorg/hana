/*!
@file
Defines `boost::hana::Sequence`.

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_SEQUENCE_HPP
#define BOOST_HANA_CONCEPT_SEQUENCE_HPP

#include <boost/hana/fwd/concept/sequence.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/integral_constant.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename S, bool condition>
    struct Sequence<S, when<condition>>
        : hana::integral_constant<bool, false>
    { };
    //! @endcond
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_SEQUENCE_HPP
