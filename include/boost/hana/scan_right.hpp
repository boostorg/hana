/*!
@file
Defines `boost::hana::scan_right`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SCAN_RIGHT_HPP
#define BOOST_HANA_SCAN_RIGHT_HPP

#include <boost/hana/fwd/scan_right.hpp>

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
    constexpr auto scan_right_t::operator()(Xs&& xs, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using ScanRight = BOOST_HANA_DISPATCH_IF(scan_right_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::scan_right(xs, f) requires 'xs' to be a Sequence");
    #endif

        return ScanRight::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
    }

    template <typename Xs, typename State, typename F>
    constexpr auto scan_right_t::operator()(Xs&& xs, State&& state, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using ScanRight = BOOST_HANA_DISPATCH_IF(scan_right_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::scan_right(xs, state, f) requires 'xs' to be a Sequence");
    #endif

        return ScanRight::apply(static_cast<Xs&&>(xs),
                                static_cast<State&&>(state),
                                static_cast<F&&>(f));
    }
    //! @endcond

    template <typename S, bool condition>
    struct scan_right_impl<S, when<condition>> : default_ {
        // with initial state
        struct scan_right_helper {
            template <typename Xs, typename State, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
                auto rest = scan_right_impl::apply(hana::drop_front(xs), state, f);
                return hana::prepend(rest, f(hana::front(xs), hana::front(rest)));
            }
        };

        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs&& xs, State state, F&& f) {
            auto done = hana::is_empty(xs);
            return hana::eval_if(done,
                hana::make_lazy(hana::lift<S>(state)),
                hana::make_lazy(scan_right_helper{})(static_cast<Xs&&>(xs), state,
                                                     static_cast<F&&>(f))
            );
        }

        // without initial state
        struct scanr1_helper2 {
            template <typename Y, typename Ys, typename F>
            constexpr decltype(auto) operator()(Y&& y, Ys&& ys, F&& f) const {
                auto rest = scan_right_impl::apply(ys, f);
                return hana::prepend(rest, f(y, hana::front(rest)));
            }
        };

        struct scanr1_helper1 {
            template <typename Xs, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
                auto y = hana::front(xs);
                auto ys = hana::drop_front(xs);
                return hana::eval_if(hana::is_empty(ys),
                    hana::make_lazy(hana::lift<S>(y)),
                    hana::make_lazy(scanr1_helper2{})(y, ys, static_cast<F&&>(f))
                );
            }
        };

        template <typename Xs, typename F>
        static constexpr auto apply(Xs&& xs, F&& f) {
            auto done = hana::is_empty(xs);
            return hana::eval_if(done,
                hana::make_lazy(hana::empty<S>()),
                hana::make_lazy(scanr1_helper1{})(static_cast<Xs&&>(xs),
                                                  static_cast<F&&>(f))
            );
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_SCAN_RIGHT_HPP
