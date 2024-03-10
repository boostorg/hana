/*!
@file
Defines `boost::hana::intersection`.

Copyright Louis Dionne 2013-2022
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTERSECTION_HPP
#define BOOST_HANA_INTERSECTION_HPP

#include <boost/hana/fwd/intersection.hpp>

#include <boost/hana/concept/set_theoretic.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Ys>
    constexpr auto intersection_t::operator()(Xs&& xs, Ys&& ys) const {
        using TX = typename hana::tag_of<Xs>::type;
        using TY = typename hana::tag_of<Ys>::type;
        using Intersection = BOOST_HANA_DISPATCH_IF(intersection_impl<TX>,
            hana::SetTheoretic<TX>::value &&
            std::is_same<TX, TY>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::SetTheoretic<TX>::value,
        "hana::intersection(xs, ys) requires 'xs' to be SetTheoretic");

        static_assert(std::is_same<TX, TY>::value,
        "hana::intersection(xs, ys) requires 'xs' and 'ys' to be of the same type");
    #endif

        return Intersection::apply(static_cast<Xs&&>(xs), static_cast<Ys&&>(ys));
    }
    //! @endcond

    template <typename S, bool condition>
    struct intersection_impl<S, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTERSECTION_HPP
