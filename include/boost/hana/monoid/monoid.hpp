/*!
@file
Forward declares `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_MONOID_HPP
#define BOOST_HANA_MONOID_MONOID_HPP

#include <boost/hana/monoid/detail/monoid_fwd.hpp>

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! Instance of `Monoid` for objects with numeric data types.
    //!
    //! Any two objects whose data types can be added with the usual
    //! `operator+` and for which a valid conversion from `int` exists
    //! (for both data types) naturally form an additive `Monoid`, with
    //! `0` being the identity and the usual `operator+` being the
    //! associative operation.
    template <typename X, typename Y>
    struct Monoid::instance<X, Y, when<is_valid<decltype((void)(
        static_cast<X>(0),
        static_cast<Y>(0),
        *(X*)0 + *(Y*)0
    ))>>> : Monoid::mcd {
        static constexpr auto plus_impl(X x, Y y)
        { return x + y; }

        // Will never be used with two different `X` and `Y` anyway.
        static constexpr auto zero_impl()
        { return static_cast<X>(0); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_MONOID_HPP
