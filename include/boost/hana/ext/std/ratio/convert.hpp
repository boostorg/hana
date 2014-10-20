/*!
@file
Defines conversions to `std::ratio`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_CONVERT_HPP
#define BOOST_HANA_EXT_STD_RATIO_CONVERT_HPP

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/ext/std/ratio/ratio.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <typename I>
    struct convert<StdRatio, I, when<is_an<IntegralConstant, I>()>> {
        template <typename N>
        static constexpr auto apply(N n) {
            constexpr auto v = value(n);
            return std::ratio<v>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_CONVERT_HPP
