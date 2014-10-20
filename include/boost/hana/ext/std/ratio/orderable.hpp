/*!
@file
Defines the instance of `boost::hana::Orderable` for `std::ratio`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_ORDERABLE_HPP
#define BOOST_HANA_EXT_STD_RATIO_ORDERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/ext/std/ratio/ratio.hpp>
#include <boost/hana/orderable/less_mcd.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct Orderable::instance<StdRatio, StdRatio> : Orderable::less_mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) less_impl(R1, R2) {
            return bool_<std::ratio_less<R1, R2>::value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_ORDERABLE_HPP
