/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_HPP
#define BOOST_HANA_MONAD_HPP

#include <boost/hana/fwd/monad.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // chain
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct chain_impl : chain_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct chain_impl<M, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::flatten(hana::transform(static_cast<Xs&&>(xs),
                                                 static_cast<F&&>(f)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // flatten
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct flatten_impl : flatten_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct flatten_impl<M, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::chain(static_cast<Xs&&>(xs), id); }
    };

    //////////////////////////////////////////////////////////////////////////
    // monadic_compose
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        struct monadic_compose_helper {
            template <typename F, typename G, typename X>
            constexpr decltype(auto) operator()(F&& f, G&& g, X&& x) const {
                using M = typename datatype<decltype(g(x))>::type;

            #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
                static_assert(_models<Monad, M>{},
                "hana::monadic_compose(f, g) requires g to return a monadic value");
            #endif

                return hana::chain(static_cast<G&&>(g)(static_cast<X&&>(x)),
                                   static_cast<F&&>(f));
            }
        };
    }

    //! @cond
    template <typename F, typename G>
    constexpr decltype(auto) _monadic_compose::operator()(F&& f, G&& g) const {
        return hana::partial(detail::monadic_compose_helper{},
            static_cast<F&&>(f),
            static_cast<G&&>(g)
        );
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // then
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct then_impl : then_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct then_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::chain(static_cast<Xs&&>(xs),
                               hana::always(static_cast<Ys&&>(ys)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // tap
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct tap_impl : tap_impl<M, when<true>> { };

    namespace detail {
        template <typename M>
        struct tap_helper {
            template <typename F, typename X>
            constexpr decltype(auto) operator()(F&& f, X&& x) const {
                (void)static_cast<F&&>(f)(x);
                return hana::lift<M>(static_cast<X&&>(x));
            }
        };
    }

    template <typename M, bool condition>
    struct tap_impl<M, when<condition>> : default_ {
        template <typename F>
        static constexpr decltype(auto) apply(F&& f)
        { return hana::partial(detail::tap_helper<M>{}, static_cast<F&&>(f)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct models_impl<Monad, M>
        : _integral_constant<bool,
            !is_default<flatten_impl<M>>{}() ||
            !is_default<chain_impl<M>>{}()
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
                hana::partial(transform, static_cast<X&&>(x))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
