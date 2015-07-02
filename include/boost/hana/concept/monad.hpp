/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_MONAD_HPP
#define BOOST_HANA_CONCEPT_MONAD_HPP

#include <boost/hana/fwd/concept/monad.hpp>

#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/concept/functor.hpp>

#include <boost/hana/chain.hpp>
#include <boost/hana/flatten.hpp>
#include <boost/hana/monadic_compose.hpp>
#include <boost/hana/tap.hpp>
#include <boost/hana/then.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct models_impl<Monad, M>
        : _integral_constant<bool,
            !is_default<flatten_impl<M>>::value ||
            !is_default<chain_impl<M>>::value
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Monad::ap_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct Monad::ap_impl {
        template <typename F, typename X>
        static constexpr decltype(auto) apply(F&& f, X&& x) {
            return hana::chain(
                static_cast<F&&>(f),
                hana::partial(hana::transform, static_cast<X&&>(x))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_MONAD_HPP
