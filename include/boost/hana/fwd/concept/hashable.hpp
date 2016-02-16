/*!
@file
Forward declares `boost::hana::Hashable`.

@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_HASHABLE_HPP
#define BOOST_HANA_FWD_CONCEPT_HASHABLE_HPP

#include <boost/hana/config.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @ingroup group-concepts
    //! The `Hashable` concept represents compile-time values that can be
    //! normalized to type-level hash values for use in associative data
    //! structures.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! By default a type's hash value is its identity and is therefore
    //! `Hashable`. Otherwise, `hash` can be specialized.
    template <typename T>
    struct Hashable;
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_FWD_CONCEPT_HASHABLE_HPP
