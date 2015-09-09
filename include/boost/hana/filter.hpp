/*!
@file
Defines `boost::hana::filter`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FILTER_HPP
#define BOOST_HANA_FILTER_HPP

#include <boost/hana/fwd/filter.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/chain.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/detail/array.hpp>
#include <boost/hana/empty.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/lift.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto filter_t::operator()(Xs&& xs, Pred&& pred) const {
        using M = typename hana::tag_of<Xs>::type;
        using Filter = BOOST_HANA_DISPATCH_IF(filter_impl<M>,
            MonadPlus<M>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(MonadPlus<M>::value,
        "hana::filter(xs, pred) requires 'xs' to be a MonadPlus");
    #endif

        return Filter::apply(static_cast<Xs&&>(xs),
                             static_cast<Pred&&>(pred));
    }
    //! @endcond

    namespace detail {
        template <typename Pred, typename M>
        struct lift_or_empty {
            template <typename X>
            static constexpr auto helper(X&& x, hana::true_)
            { return hana::lift<M>(static_cast<X&&>(x)); }

            template <typename X>
            static constexpr auto helper(X&&, hana::false_)
            { return hana::empty<M>(); }

            template <typename X>
            constexpr auto operator()(X&& x) const {
                constexpr bool cond = decltype(std::declval<Pred>()(x))::value;
                return helper(static_cast<X&&>(x), hana::bool_c<cond>);
            }
        };
    }

    template <typename M, bool condition>
    struct filter_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred const&) {
            return hana::chain(static_cast<Xs&&>(xs),
                detail::lift_or_empty<Pred, M>{}
            );
        }
    };

    namespace detail {
        template <bool ...b>
        struct filter_central {
            static constexpr bool bs[] = {b...};
            static constexpr std::size_t filtered_size =
                        detail::count(bs, bs + sizeof(bs), true);

            static constexpr auto get_indices() {
                detail::array<std::size_t, filtered_size> indices{};
                std::size_t* keep = &indices[0];
                for (std::size_t i = 0; i < sizeof...(b); ++i)
                    if (bs[i])
                        *keep++ = i;
                return indices;
            }

            template <typename S, typename Xs, std::size_t ...i>
            static constexpr auto finish(Xs&& xs, std::index_sequence<i...>) {
                constexpr auto keep = get_indices();
                (void)keep; // workaround GCC warning when sizeof...(i) == 0
                return hana::make<S>(
                    hana::at_c<keep[i]>(static_cast<Xs&&>(xs))...
                );
            }
        };
    }

    template <typename M>
    struct filter_impl<M, when<Sequence<M>::value>> {
        template <typename Pred, typename Xs, std::size_t ...i>
        static constexpr auto filter_indices(Xs&& xs, std::index_sequence<i...>) {
            using info = detail::filter_central<decltype(
                std::declval<Pred>()(hana::at_c<i>(static_cast<Xs&&>(xs)))
            )::value...>;
            return info::template finish<M>(static_cast<Xs&&>(xs),
                std::make_index_sequence<info::filtered_size>{});
        }

        template <typename Pred, typename Xs>
        static constexpr auto filter_indices(Xs&&, std::index_sequence<>)
        { return hana::make<M>(); }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred const&) {
            constexpr std::size_t len = decltype(hana::length(xs))::value;
            return filter_indices<Pred>(static_cast<Xs&&>(xs),
                                        std::make_index_sequence<len>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FILTER_HPP
