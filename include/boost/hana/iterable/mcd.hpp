/*!
@file
Defines `boost::hana::Iterable::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_MCD_HPP
#define BOOST_HANA_ITERABLE_MCD_HPP

#include <boost/hana/iterable/iterable.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `head`, `tail` and `is_empty`
    struct Iterable::mcd {
        template <typename Index, typename Iterable_>
        static constexpr auto at_impl(Index n, Iterable_ iterable) {
            return eval_if(equal(n, size_t<0>),
                [=](auto _) { return head(_(iterable)); },
                [=](auto _) { return at_impl(_(n) - size_t<1>, tail(_(iterable))); }
            );
        }

        template <typename Iterable_>
        static constexpr auto last_impl(Iterable_ iterable) {
            return eval_if(is_empty(tail(iterable)),
                [=](auto _) { return head(_(iterable)); },
                [=](auto _) { return last_impl(tail(_(iterable))); }
            );
        }

        template <typename N, typename Iterable_>
        static constexpr auto drop_impl(N n, Iterable_ iterable) {
            return eval_if(or_(equal(n, size_t<0>), is_empty(iterable)),
                [=](auto) { return iterable; },
                [=](auto _) { return drop_impl(_(n) - size_t<1>, tail(_(iterable))); }
            );
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto drop_while_impl(Pred pred, Iterable_ iterable) {
            return eval_if(is_empty(iterable),
                [=](auto) { return iterable; },
                [=](auto _) {
                    return eval_if(pred(_(head)(iterable)),
                        [=](auto _) { return drop_while_impl(pred, _(tail)(iterable)); },
                        [=](auto) { return iterable; }
                    );
                }
            );
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto drop_until_impl(Pred pred, Iterable_ iterable) {
            return drop_while([=](auto x) { return not_(pred(x)); }, iterable);
        }

        template <typename It, typename F>
        static constexpr auto for_each_impl(It it, F f) {
            return eval_if(is_empty(it),
                [](auto) { },
                [=](auto _) {
                    f(_(head)(it));
                    for_each_impl(_(tail)(it), f);
                }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_MCD_HPP
