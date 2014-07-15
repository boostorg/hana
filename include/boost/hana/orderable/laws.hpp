/*!
@file
Defines `boost::hana::Orderable::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_LAWS_HPP
#define BOOST_HANA_ORDERABLE_LAWS_HPP

#include <boost/hana/orderable/orderable.hpp>

#include <boost/hana/bool.hpp>


namespace boost { namespace hana {
    //! @details
    //! `less` must define a strict weak ordering. Formally, let
    //! @code
    //!     x ~ y if and only if !(x < y) && !(y < x)
    //! @endcode
    //! be the incomparability relation. Then, for all `a`, `b`, `c` of an
    //! orderable data type,
    //! @code
    //!     !(a < a)                     // Irreflexivity
    //!     if a < b then !(b < a)       // Asymmetry
    //!     if a < b && b < c then a < c // Transitivity
    //!     if a ~ b && b ~ c then a ~ c // Transitivity of incomparability
    //! @endcode
    //!
    //! @todo Actually implement the laws.
    struct Orderable::laws {
        static constexpr auto check()
        { return true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_LAWS_HPP
