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
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename Xs, typename F, typename = typename detail::std::enable_if<
            has_operator<datatype_t<Xs>, decltype(bind)>::value
        >::type>
        constexpr decltype(auto) operator|(Xs&& xs, F&& f) {
            return hana::bind(static_cast<Xs&&>(xs),
                              static_cast<F&&>(f));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // bind
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct bind_impl : bind_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct bind_impl<M, when<condition>> : default_ {
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
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::bind(static_cast<Xs&&>(xs), id);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // mcompose
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct mcompose_impl : mcompose_impl<M, when<true>> { };

    namespace monad_detail {
        template <typename F, typename G>
        struct mcompose {
            F f; G g;
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) const& {
                return hana::bind(f(static_cast<X&&>(x)), g);
            }
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) & {
                return hana::bind(f(static_cast<X&&>(x)), g);
            }
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) && {
                return hana::bind(detail::std::move(f)(static_cast<X&&>(x)),
                                  detail::std::move(g));
            }
        };
    }

    template <typename M, bool condition>
    struct mcompose_impl<M, when<condition>> : default_ {
        template <typename F, typename G>
        static constexpr decltype(auto) apply(F&& f, G&& g) {
            return detail::create<monad_detail::mcompose>{}(
                static_cast<F&&>(f), static_cast<G&&>(g));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // then
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct then_impl : then_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct then_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::bind(static_cast<Xs&&>(xs),
                              hana::always(static_cast<Ys&&>(ys)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // tap
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct tap_impl : tap_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct tap_impl<M, when<condition>> : default_ {
        template <typename F>
        struct _tap {
            F f;
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) const& {
                f(x);
                return lift<M>(static_cast<X&&>(x));
            }
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) & {
                f(x);
                return lift<M>(static_cast<X&&>(x));
            }
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) && {
                detail::std::move(f)(x);
                return lift<M>(static_cast<X&&>(x));
            }
        };

        template <typename F>
        static constexpr decltype(auto) apply(F&& f) {
            return detail::create<_tap>{}(static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct models_impl<Monad, M>
        : _integral_constant<bool,
            !is_default<flatten_impl<M>>{} ||
            !is_default<bind_impl<M>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Monad::ap_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct Monad::ap_impl {
        template <typename F, typename X>
        static constexpr decltype(auto) apply(F&& f, X&& x) {
            return hana::bind(
                static_cast<F&&>(f),
                hana::partial(transform, static_cast<X&&>(x))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
