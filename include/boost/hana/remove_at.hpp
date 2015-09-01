/*!
@file
Defines `boost::hana::remove_at` and `boost::hana::remove_at_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_REMOVE_AT_HPP
#define BOOST_HANA_REMOVE_AT_HPP

#include <boost/hana/fwd/remove_at.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename N>
    constexpr auto remove_at_t::operator()(Xs&& xs, N&& n) const {
        using S = typename datatype<Xs>::type;
        using RemoveAt = BOOST_HANA_DISPATCH_IF(remove_at_impl<S>,
            _models<Sequence, S>::value &&
            _models<Constant, N>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Sequence, S>::value,
        "hana::remove_at(xs, n) requires 'xs' to be a Sequence");

        static_assert(_models<Constant, N>::value,
        "hana::remove_at(xs, n) requires 'n' to be a Constant");
    #endif

        return RemoveAt::apply(static_cast<Xs&&>(xs), static_cast<N&&>(n));
    }
    //! @endcond

    template <typename S, bool condition>
    struct remove_at_impl<S, when<condition>> : default_ {
        template <typename Xs, std::size_t ...before, std::size_t ...after>
        static constexpr auto
        remove_at_helper(Xs&& xs, std::index_sequence<before...>,
                                  std::index_sequence<after...>)
        {
            return hana::make<S>(
                hana::at_c<before>(static_cast<Xs&&>(xs))...,
                hana::at_c<after + sizeof...(before) + 1>(static_cast<Xs&&>(xs))...
            );
        }

        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr std::size_t n = hana::value<N>();
            constexpr std::size_t len = hana::value<decltype(hana::length(xs))>();
            return remove_at_helper(static_cast<Xs&&>(xs),
                                    std::make_index_sequence<n>{},
                                    std::make_index_sequence<len - n - 1>{});
        }
    };

    template <std::size_t n>
    struct remove_at_c_t {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return hana::remove_at(static_cast<Xs&&>(xs), hana::size_c<n>); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_REMOVE_AT_HPP
