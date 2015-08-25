/*!
@file
Defines `boost::hana::back`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BACK_HPP
#define BOOST_HANA_BACK_HPP

#include <boost/hana/fwd/back.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs>
    constexpr decltype(auto) back_t::operator()(Xs&& xs) const {
        using It = typename datatype<Xs>::type;
        using Back = BOOST_HANA_DISPATCH_IF(back_impl<It>,
            _models<Iterable, It>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Iterable, It>::value,
        "hana::back(xs) requires 'xs' to be an Iterable");
    #endif

        return Back::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    template <typename It, bool condition>
    struct back_impl<It, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            constexpr std::size_t len = hana::value<decltype(hana::length(xs))>();
            static_assert(len > 0, "hana::back(xs) requires 'xs' to be non-empty");
            return hana::at(static_cast<Xs&&>(xs), hana::size_t<len - 1>);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BACK_HPP
