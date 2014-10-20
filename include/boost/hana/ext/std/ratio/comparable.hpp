/*!
@file
Defines the instance of `boost::hana::Comparable` for `std::ratio`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_COMPARABLE_HPP
#define BOOST_HANA_EXT_STD_RATIO_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/ext/std/ratio/ratio.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<StdRatio, StdRatio> : Comparable::equal_mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) equal_impl(R1, R2) {
            return bool_<std::ratio_equal<R1, R2>::value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_COMPARABLE_HPP
