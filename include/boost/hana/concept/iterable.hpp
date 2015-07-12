/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_ITERABLE_HPP
#define BOOST_HANA_CONCEPT_ITERABLE_HPP

#include <boost/hana/fwd/concept/iterable.hpp>

//! @todo Get rid of those includes
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/concept/enumerable.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/iterate.hpp>
#include <boost/hana/functional/lockstep.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/concept/monoid.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/concept/searchable.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/back.hpp>
#include <boost/hana/drop_front.hpp>
#include <boost/hana/drop_front_exactly.hpp>
#include <boost/hana/drop_while.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/tail.hpp>

#include <cstddef>


namespace boost { namespace hana {
    template <typename It>
    struct models_impl<Iterable, It>
        : _integral_constant<bool,
            !is_default<at_impl<It>>{}() &&
            !is_default<tail_impl<It>>{}() &&
            !is_default<is_empty_impl<It>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename It>
    struct Iterable::find_if_impl {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::only_when(hana::compose(not_, is_empty), hana::front,
                hana::drop_while(static_cast<Xs&&>(xs),
                                 hana::compose(hana::not_, static_cast<Pred&&>(pred)))
            );
        }
    };

    template <typename It>
    struct Iterable::any_of_impl {
        template <bool Done, typename Dummy = void>
        struct any_of_helper {
            template <typename Xs, typename Pred>
            static constexpr auto apply(bool prev_cond, Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::front(xs)), true_, false_);
                decltype(auto) tail = hana::tail(static_cast<Xs&&>(xs));
                constexpr bool done = hana::value<decltype(hana::is_empty(tail))>();
                return prev_cond ? true_
                    : any_of_impl::any_of_helper<done>::apply(cond,
                                static_cast<decltype(tail)&&>(tail),
                                static_cast<Pred&&>(pred));
            }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(true_), Xs&&, Pred&&)
            { return true_; }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(false_), Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::front(xs)), true_, false_);
                constexpr bool done = hana::value<decltype(
                    hana::is_empty(hana::tail(xs))
                )>();
                return any_of_impl::any_of_helper<done>::apply(cond,
                                        hana::tail(static_cast<Xs&&>(xs)),
                                        static_cast<Pred&&>(pred));
            }
        };

        template <typename Dummy>
        struct any_of_helper<true, Dummy> {
            template <typename Cond, typename Xs, typename Pred>
            static constexpr auto apply(Cond cond, Xs&&, Pred&&)
            { return cond; }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr bool done = hana::value<decltype(hana::is_empty(xs))>();
            return any_of_impl::any_of_helper<done>::apply(false_,
                                            static_cast<Xs&&>(xs),
                                            static_cast<Pred&&>(pred));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ITERABLE_HPP
