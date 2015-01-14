/*!
@file
Defines `boost::hana::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_HPP

#include <boost/hana/fwd/functor.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    template <typename Functor, typename _>
    struct fmap_impl<Functor, when<is_implemented<adjust_impl<Functor>, _>>, _> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return adjust(
                detail::std::forward<Xs>(xs),
                always(true_),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename Functor, typename Context>
    struct adjust_impl<Functor, when<
        is_implemented<fmap_impl<Functor>, Context>
    >, Context> {
        template <typename Xs, typename Pred, typename F>
        static constexpr auto apply(Xs xs, Pred pred, F f) {
            auto go = [=](auto x) {
                return eval_if(pred(x),
                    [=](auto _) -> decltype(auto) { return _(f)(x); },
                    always(x)
                );
            };
            return fmap(xs, go);
        }
    };

    template <typename Functor, typename _>
    struct replace_impl<Functor, when<is_implemented<adjust_impl<Functor>, _>>, _> {
        template <typename F, typename Pred, typename Value>
        static constexpr decltype(auto) apply(F&& functor, Pred&& pred, Value&& v) {
            return adjust(
                detail::std::forward<F>(functor),
                detail::std::forward<Pred>(pred),
                always(detail::std::forward<Value>(v))
            );
        }
    };

    template <typename Functor, typename _>
    struct fill_impl<Functor, when<is_implemented<fmap_impl<Functor>, _>>, _> {
        template <typename F, typename Value>
        static constexpr decltype(auto) apply(F&& functor, Value&& v) {
            return fmap(
                detail::std::forward<F>(functor),
                always(detail::std::forward<Value>(v))
            );
        }
    };

    template <typename F>
    constexpr auto is_a<Functor, F> = bool_<
        is_implemented<fmap_impl<F>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
