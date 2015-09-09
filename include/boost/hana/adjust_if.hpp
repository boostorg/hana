/*!
@file
Defines `boost::hana::adjust_if`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ADJUST_IF_HPP
#define BOOST_HANA_ADJUST_IF_HPP

#include <boost/hana/fwd/adjust_if.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/transform.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Pred, typename F>
    constexpr auto adjust_if_t::operator()(Xs&& xs, Pred&& pred, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using AdjustIf = BOOST_HANA_DISPATCH_IF(adjust_if_impl<S>,
            Functor<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Functor<S>::value,
        "hana::adjust_if(xs, pred, f) requires 'xs' to be a Functor");
    #endif

        return AdjustIf::apply(static_cast<Xs&&>(xs),
                               static_cast<Pred&&>(pred),
                               static_cast<F&&>(f));
    }
    //! @endcond

    namespace detail {
        struct adjust_if_helper {
            template <typename F, typename X>
            static constexpr decltype(auto) helper(hana::true_, F&& f, X&& x)
            { return static_cast<F&&>(f)(static_cast<X&&>(x)); }

            template <typename F, typename X>
            static constexpr X helper(hana::false_, F&&, X&& x)
            { return static_cast<X&&>(x); }

            template <typename F, typename X>
            static constexpr decltype(auto) helper(bool cond, F&& f, X&& x) {
                return cond ? static_cast<F&&>(f)(static_cast<X&&>(x))
                            : static_cast<X&&>(x);
            }

            template <typename Pred, typename F, typename X>
            constexpr decltype(auto) operator()(Pred&& pred, F&& f, X&& x) const {
                auto cond = hana::if_(static_cast<Pred&&>(pred)(x),
                    hana::true_c, hana::false_c
                );
                return adjust_if_helper::helper(cond, static_cast<F&&>(f),
                                                      static_cast<X&&>(x));
            }
        };
    }

    template <typename Fun, bool condition>
    struct adjust_if_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename Pred, typename F>
        static constexpr auto apply(Xs&& xs, Pred&& pred, F&& f) {
            return hana::transform(static_cast<Xs&&>(xs),
                hana::partial(detail::adjust_if_helper{},
                              static_cast<Pred&&>(pred),
                              static_cast<F&&>(f)));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ADJUST_IF_HPP
