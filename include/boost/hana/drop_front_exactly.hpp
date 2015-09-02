/*!
@file
Defines `boost::hana::drop_front_exactly`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DROP_FRONT_EXACTLY_HPP
#define BOOST_HANA_DROP_FRONT_EXACTLY_HPP

#include <boost/hana/fwd/drop_front_exactly.hpp>

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/functional/iterate.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tail.hpp>
#include <boost/hana/value.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename N>
    constexpr auto drop_front_exactly_t::operator()(Xs&& xs, N&& n) const {
        using It = typename datatype<Xs>::type;
        using DropFrontExactly = BOOST_HANA_DISPATCH_IF(
            drop_front_exactly_impl<It>,
            _models<Iterable, It>::value &&
            _models<Constant, N>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Iterable, It>::value,
        "hana::drop_front_exactly(xs, n) requires 'xs' to be an Iterable");

        static_assert(_models<Constant, N>::value,
        "hana::drop_front_exactly(xs, n) requires 'n' to be a Constant");
    #endif

        return DropFrontExactly::apply(static_cast<Xs&&>(xs),
                                       static_cast<N&&>(n));
    }

    template <typename Xs>
    constexpr auto drop_front_exactly_t::operator()(Xs&& xs) const {
        return (*this)(static_cast<Xs&&>(xs), hana::size_c<1>);
    }
    //! @endcond

    template <typename It, bool condition>
    struct drop_front_exactly_impl<It, when<condition>> : default_ {
        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr auto n = hana::value<N>();
            return hana::iterate<n>(hana::tail)(static_cast<Xs&&>(xs));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DROP_FRONT_EXACTLY_HPP
