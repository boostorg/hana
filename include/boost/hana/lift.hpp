/*!
@file
Defines `boost::hana::lift`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIFT_HPP
#define BOOST_HANA_LIFT_HPP

#include <boost/hana/fwd/lift.hpp>

#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>


namespace boost { namespace hana {
    template <typename A>
    struct lift_t {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Applicative<A>::value,
        "hana::lift<A> requires 'A' to be an Applicative");
    #endif

        template <typename X>
        constexpr auto operator()(X&& x) const {
            using Lift = BOOST_HANA_DISPATCH_IF(lift_impl<A>,
                Applicative<A>::value
            );

            return Lift::apply(static_cast<X&&>(x));
        }
    };

    template <typename A, bool condition>
    struct lift_impl<A, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...args) = delete;
    };

    template <typename S>
    struct lift_impl<S, when<Sequence<S>::value>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::make<S>(static_cast<X&&>(x)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIFT_HPP
