/*!
@file
Defines `boost::hana::Applicative`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_APPLICATIVE_HPP
#define BOOST_HANA_CONCEPT_APPLICATIVE_HPP

#include <boost/hana/fwd/concept/applicative.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/concept/functor.hpp>

#include <boost/hana/ap.hpp>
#include <boost/hana/lift.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Applicative::transform_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename A>
    struct Applicative::transform_impl {
        template <typename X, typename F>
        static constexpr decltype(auto) apply(X&& x, F&& f) {
            return hana::ap(lift<A>(static_cast<F&&>(f)),
                            static_cast<X&&>(x));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename A>
    struct models_impl<Applicative, A>
        : _integral_constant<bool,
            !is_default<ap_impl<A>>::value &&
            !is_default<lift_impl<A>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_APPLICATIVE_HPP
