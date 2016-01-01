/*!
@file
Defines `boost::hana::scan_left`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SCAN_LEFT_HPP
#define BOOST_HANA_SCAN_LEFT_HPP

#include <boost/hana/fwd/scan_left.hpp>

#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/drop_front.hpp>
#include <boost/hana/empty.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/lift.hpp>
#include <boost/hana/prepend.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename F>
    constexpr auto scan_left_t::operator()(Xs&& xs, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using ScanLeft = BOOST_HANA_DISPATCH_IF(scan_left_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::scan_left(xs, f) requires 'xs' to be a Sequence");
    #endif

        return ScanLeft::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
    }

    template <typename Xs, typename State, typename F>
    constexpr auto scan_left_t::operator()(Xs&& xs, State&& state, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using ScanLeft = BOOST_HANA_DISPATCH_IF(scan_left_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::scan_left(xs, state, f) requires 'xs' to be a Sequence");
    #endif

        return ScanLeft::apply(static_cast<Xs&&>(xs),
                               static_cast<State&&>(state),
                               static_cast<F&&>(f));
    }
    //! @endcond

    template <typename S, bool condition>
    struct scan_left_impl<S, when<condition>> : default_ {
        struct scan_left_helper {
            template <typename Xs, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
                //! @todo We need a way to extract the front of an Iterable
                //! and get its tail at the same time. It would allow us to
                //! use perfect forwarding here.
                return hana::scan_left(hana::drop_front(xs), hana::front(xs),
                                       static_cast<F&&>(f));
            }

            template <typename Xs, typename State, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
                return hana::prepend(
                    scan_left_impl::apply(
                        hana::drop_front(xs),
                        f(state, hana::front(xs)),
                        f
                    ),
                    state
                );
            }
        };

        // with initial state
        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State state, F f) {
            return hana::eval_if(hana::is_empty(xs),
                hana::make_lazy(hana::lift<S>(state)),
                hana::make_lazy(scan_left_helper{})(xs, state, f)
            );
        }

        // without initial state
        template <typename Xs, typename F>
        static constexpr auto apply(Xs&& xs, F&& f) {
            decltype(auto) done = hana::is_empty(xs);
            return hana::eval_if(static_cast<decltype(done)&&>(done),
                hana::make_lazy(hana::empty<S>()),
                hana::make_lazy(scan_left_helper{})(
                    static_cast<Xs&&>(xs), static_cast<F&&>(f)
                )
            );
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_SCAN_LEFT_HPP
