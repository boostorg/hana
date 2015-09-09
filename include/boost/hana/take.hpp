/*!
@file
Defines `boost::hana::take` and `boost::hana::take_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TAKE_HPP
#define BOOST_HANA_TAKE_HPP

#include <boost/hana/fwd/take.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename N>
    constexpr auto take_t::operator()(Xs&& xs, N const& n) const {
        using S = typename hana::tag_of<Xs>::type;
        using Take = BOOST_HANA_DISPATCH_IF(take_impl<S>,
            Sequence<S>::value &&
            IntegralConstant<N>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Sequence<S>::value,
        "hana::take(xs, n) requires 'xs' to be a Sequence");

        static_assert(IntegralConstant<N>::value,
        "hana::take(xs, n) requires 'n' to be an IntegralConstant");
    #endif

        return Take::apply(static_cast<Xs&&>(xs), n);
    }
    //! @endcond

    template <typename S, bool condition>
    struct take_impl<S, when<condition>> : default_ {
        template <typename Xs, std::size_t ...n>
        static constexpr auto take_helper(Xs&& xs, std::index_sequence<n...>) {
            return hana::make<S>(hana::at_c<n>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr std::size_t n = N::value;
            constexpr std::size_t size = decltype(hana::length(xs))::value;
            return take_helper(static_cast<Xs&&>(xs),
                        std::make_index_sequence<(n < size ? n : size)>{});
        }
    };

    template <std::size_t n>
    struct take_c_t {
        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const {
            return hana::take(static_cast<Xs&&>(xs), hana::size_c<n>);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TAKE_HPP
