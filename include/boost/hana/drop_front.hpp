/*!
@file
Defines `boost::hana::drop_front`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DROP_FRONT_HPP
#define BOOST_HANA_DROP_FRONT_HPP

#include <boost/hana/fwd/drop_front.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/iterate.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/tail.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename N>
    constexpr auto drop_front_t::operator()(Xs&& xs, N const& n) const {
        using It = typename hana::tag_of<Xs>::type;
        using DropFront = BOOST_HANA_DISPATCH_IF(drop_front_impl<It>,
            Iterable<It>::value &&
            IntegralConstant<N>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Iterable<It>::value,
        "hana::drop_front(xs, n) requires 'xs' to be an Iterable");

        static_assert(IntegralConstant<N>::value,
        "hana::drop_front(xs, n) requires 'n' to be an IntegralConstant");
    #endif

        return DropFront::apply(static_cast<Xs&&>(xs), n);
    }

    template <typename Xs>
    constexpr auto drop_front_t::operator()(Xs&& xs) const {
        return (*this)(static_cast<Xs&&>(xs), hana::size_c<1>);
    }
    //! @endcond

    namespace detail {
        struct maybe_tail {
            template <typename Xs>
            constexpr auto operator()(Xs&& xs) const {
                return hana::if_(hana::is_empty(xs), hana::id, hana::tail)(
                    static_cast<Xs&&>(xs)
                );
            }
        };
    }

    template <typename It, bool condition>
    struct drop_front_impl<It, when<condition>> : default_ {
        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr std::size_t n = N::value;
            return hana::iterate<n>(detail::maybe_tail{}, static_cast<Xs&&>(xs));
        }
    };

    template <typename It>
    struct drop_front_impl<It, when<Sequence<It>::value>> {
        template <std::size_t n, typename Xs, std::size_t ...i>
        static constexpr auto drop_front_helper(Xs&& xs, std::index_sequence<i...>) {
            return hana::make<It>(
                hana::at_c<n + i>(static_cast<Xs&&>(xs))...
            );
        }

        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr std::size_t n = N::value;
            constexpr std::size_t len = decltype(hana::length(xs))::value;
            return drop_front_helper<n>(static_cast<Xs&&>(xs),
                    std::make_index_sequence<(n < len ? len - n : 0)>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DROP_FRONT_HPP
