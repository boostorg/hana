/*!
@file
Defines the instance of `boost::hana::Group` for `std::ratio`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_GROUP_HPP
#define BOOST_HANA_EXT_STD_RATIO_GROUP_HPP

#include <boost/hana/ext/std/ratio/ratio.hpp>
#include <boost/hana/group/minus_mcd.hpp>

// Mcd
#include <boost/hana/ext/std/ratio/monoid.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct Group::instance<StdRatio, StdRatio>
        : Group::minus_mcd<StdRatio, StdRatio>
    {
        template <typename R1, typename R2>
        static constexpr decltype(auto) minus_impl(R1, R2) {
            return std::ratio_subtract<R1, R2>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_GROUP_HPP
