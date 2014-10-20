/*!
@file
Defines the instance of `boost::hana::IntegralDomain` for `std::ratio`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_EXT_STD_RATIO_INTEGRAL_DOMAIN_HPP

#include <boost/hana/ext/std/ratio/ratio.hpp>
#include <boost/hana/integral_domain/mcd.hpp>

// Mcd
#include <boost/hana/ext/std/ratio/ring.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct IntegralDomain::instance<StdRatio, StdRatio> : IntegralDomain::mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) quot_impl(R1, R2) {
            return std::ratio_divide<R1, R2>{};
        }

        template <typename R1, typename R2>
        static constexpr decltype(auto) mod_impl(R1, R2) {
            return std::ratio<0>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_INTEGRAL_DOMAIN_HPP
