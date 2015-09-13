/*!
@file
Defines `boost::hana::sort`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SORT_HPP
#define BOOST_HANA_SORT_HPP

#include <boost/hana/fwd/sort.hpp>

#include <boost/hana/concat.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/detail/nested_by.hpp> // required by fwd decl
#include <boost/hana/eval_if.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/partition.hpp>
#include <boost/hana/prepend.hpp>
#include <boost/hana/second.hpp>
#include <boost/hana/tail.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Predicate>
    constexpr auto sort_t::operator()(Xs&& xs, Predicate&& pred) const {
        using S = typename hana::tag_of<Xs>::type;
        using Sort = BOOST_HANA_DISPATCH_IF(sort_impl<S>,
            Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Sequence<S>::value,
        "hana::sort(xs, predicate) requires 'xs' to be a Sequence");
    #endif

        return Sort::apply(static_cast<Xs&&>(xs),
                           static_cast<Predicate&&>(pred));
    }

    template <typename Xs>
    constexpr auto sort_t::operator()(Xs&& xs) const {
        using S = typename hana::tag_of<Xs>::type;
        using Sort = BOOST_HANA_DISPATCH_IF(sort_impl<S>,
            Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Sequence<S>::value,
        "hana::sort(xs) requires 'xs' to be a Sequence");
    #endif

        return Sort::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    template <typename S, bool condition>
    struct sort_impl<S, when<condition>> : default_ {
        struct sort_by_helper2 {
            template <typename Xs, typename Pred>
            constexpr auto operator()(Xs&& xs, Pred&& pred) const {
                auto pivot = hana::front(xs);
                auto rest = hana::tail(xs);
                auto parts = hana::partition(rest,
                                    hana::partial(hana::flip(pred), pivot));
                return hana::concat(
                    sort_impl::apply(hana::first(parts), pred),
                    hana::prepend(
                        sort_impl::apply(hana::second(parts), pred),
                        pivot
                    )
                );
            }
        };

        struct sort_by_helper1 {
            template <typename Xs, typename Pred>
            constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                return hana::eval_if(hana::is_empty(hana::tail(xs)),
                    hana::make_lazy(xs),
                    hana::make_lazy(sort_by_helper2{})(xs, static_cast<Pred&&>(pred))
                );
            }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred&& pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::make_lazy(xs),
                hana::make_lazy(sort_by_helper1{})(xs, static_cast<Pred&&>(pred))
            );
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs)
        { return sort_impl::apply(static_cast<Xs&&>(xs), hana::less); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SORT_HPP
