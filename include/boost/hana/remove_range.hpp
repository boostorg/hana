/*!
@file
Defines `boost::hana::remove_range` and `boost::hana::remove_range_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_REMOVE_RANGE_HPP
#define BOOST_HANA_REMOVE_RANGE_HPP

#include <boost/hana/fwd/remove_range.hpp>

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
    template <typename Xs, typename From, typename To>
    constexpr auto remove_range_t::operator()(Xs&& xs, From&& from, To&& to) const {
        using S = typename hana::tag_of<Xs>::type;
        using RemoveRange = BOOST_HANA_DISPATCH_IF(remove_range_impl<S>,
            Sequence<S>::value &&
            Constant<From>::value &&
            Constant<To>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Sequence<S>::value,
        "hana::remove_range(xs, from, to) requires 'xs' to be a Sequence");

        static_assert(Constant<From>::value,
        "hana::remove_range(xs, from, to) requires 'from' to be a Constant");

        static_assert(Constant<To>::value,
        "hana::remove_range(xs, from, to) requires 'to' to be a Constant");
    #endif

        return RemoveRange::apply(static_cast<Xs&&>(xs),
                                  static_cast<From&&>(from),
                                  static_cast<To&&>(to));
    }
    //! @endcond

    template <typename S, bool condition>
    struct remove_range_impl<S, when<condition>> : default_ {
        template <std::size_t offset, typename Xs, std::size_t ...before, std::size_t ...after>
        static constexpr auto
        remove_range_helper(Xs&& xs, std::index_sequence<before...>,
                                     std::index_sequence<after...>)
        {
            return hana::make<S>(
                hana::at_c<before>(static_cast<Xs&&>(xs))...,
                hana::at_c<offset + after>(static_cast<Xs&&>(xs))...
            );
        }

        template <typename Xs, typename From, typename To>
        static constexpr auto apply(Xs&& xs, From const&, To const&) {
            constexpr std::size_t from = hana::value<From>();
            constexpr std::size_t to = hana::value<To>();
            constexpr std::size_t len = hana::value<decltype(hana::length(xs))>();
            constexpr std::size_t before = from == to ? len : from;
            constexpr std::size_t after = from == to ? 0 : len - to;
            static_assert(from <= to, "hana::remove_range(xs, from, to) requires 'from <= to'");
            return remove_range_helper<to>(static_cast<Xs&&>(xs),
                                           std::make_index_sequence<before>{},
                                           std::make_index_sequence<after>{});
        }
    };

    template <std::size_t from, std::size_t to>
    struct remove_range_c_t {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return hana::remove_range(static_cast<Xs&&>(xs),
                                      hana::size_c<from>,
                                      hana::size_c<to>);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_REMOVE_RANGE_HPP
