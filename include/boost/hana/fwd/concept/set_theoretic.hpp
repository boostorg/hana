/*!
@file
Forward declares `boost::hana::SetTheoretic`.

@copyright Shreyans Doshi 2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_SET_THEORETIC_HPP
#define BOOST_HANA_FWD_CONCEPT_SET_THEORETIC_HPP

#include <boost/hana/config.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @ingroup group-concepts
    //! @defgroup group-SetTheoretic SetTheoretic
    //! The `SetTheoretic` concept represents data structures supporting
    //! algebra of sets.
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `union_`, `intersection`, `difference` and `symmetric_difference`
    //!
    //! Concrete models
    //! ---------------
    //! `hana::set`, `hana::map`
    //!
    //!
    template <typename STh>
    struct SetTheoretic;
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_FWD_CONCEPT_SET_THEORETIC_HPP
