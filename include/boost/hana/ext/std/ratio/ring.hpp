/*!
@file
Defines the instance of `boost::hana::Ring` for `std::ratio`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_RING_HPP
#define BOOST_HANA_EXT_STD_RATIO_RING_HPP

#include <boost/hana/ext/std/ratio/ratio.hpp>
#include <boost/hana/ring/mcd.hpp>

// Mcd
#include <boost/hana/ext/std/ratio/group.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct Ring::instance<StdRatio, StdRatio> : Ring::mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) mult_impl(R1, R2) {
            return std::ratio_multiply<R1, R2>{};
        }

        static constexpr decltype(auto) one_impl() {
            return std::ratio<1>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_RING_HPP
