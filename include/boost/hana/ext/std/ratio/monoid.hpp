/*!
@file
Defines the instance of `boost::hana::Monoid` for `std::ratio`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_MONOID_HPP
#define BOOST_HANA_EXT_STD_RATIO_MONOID_HPP

#include <boost/hana/ext/std/ratio/ratio.hpp>
#include <boost/hana/monoid/mcd.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct Monoid::instance<StdRatio, StdRatio> : Monoid::mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) plus_impl(R1, R2) {
            return std::ratio_add<R1, R2>{};
        }

        static constexpr decltype(auto) zero_impl() {
            return std::ratio<0>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_MONOID_HPP
