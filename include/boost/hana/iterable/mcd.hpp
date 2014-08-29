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
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `head`, `tail` and `is_empty`
    struct Iterable::mcd {
        template <typename Index, typename Iterable_>
        static constexpr auto at_impl(Index n, Iterable_ iterable) {
            return eval_if(equal(n, size_t<0>),
                [=](auto _) { return head(_(iterable)); },
                [=](auto _) { return at_impl(_(minus)(n, size_t<1>), tail(_(iterable))); }
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
                [=](auto _) { return drop_impl(_(minus)(n, size_t<1>), tail(_(iterable))); }
            );
        }

        template <typename Xs, typename Pred>
        static constexpr auto drop_while_impl(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                [=](auto) { return xs; },
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) { return drop_while_impl(_(tail)(xs), pred); },
                        [=](auto) { return xs; }
                    );
                }
            );
        }

        template <typename Xs, typename Pred>
        static constexpr auto drop_until_impl(Xs xs, Pred pred) {
            return drop_while(xs, [=](auto x) { return not_(pred(x)); });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_MCD_HPP
