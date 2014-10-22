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
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `fmap`
    struct Functor::fmap_mcd : functor_detail::common {
        template <typename Xs, typename Pred, typename F>
        static constexpr auto adjust_impl(Xs xs, Pred pred, F f) {
            auto go = [=](auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return _(f)(x); },
                    [=](auto) { return x; }
                );
            };
            return fmap(xs, go);
        }
    };

    //! Minimal complete definition: `adjust`
    struct Functor::adjust_mcd : functor_detail::common {
        template <typename Xs, typename F>
        static constexpr decltype(auto) fmap_impl(Xs&& xs, F&& f) {
            return adjust(
                detail::std::forward<Xs>(xs),
                [](auto) { return true_; },
                detail::std::forward<F>(f)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
