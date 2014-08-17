/*!
@file
Defines the instance of `boost::hana::IntegralConstant` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_INTEGRAL_INTEGRAL_CONSTANT_HPP

#include <boost/hana/integral_constant/mcd.hpp>
#include <boost/hana/integral/integral.hpp>

// Mcd
#include <boost/hana/integral/constant.hpp>


namespace boost { namespace hana {
    template <>
    struct IntegralConstant::instance<Integral> : IntegralConstant::mcd {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return integral<T, v>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_INTEGRAL_CONSTANT_HPP
