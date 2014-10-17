/*!
@file
Defines the instance of `boost::hana::IntegralConstant` for
`boost::hana::BoostMplIntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_INTEGRAL_CONSTANT_HPP

#include <boost/hana/ext/boost/mpl/integral_c/integral_c.hpp>
#include <boost/hana/integral_constant/mcd.hpp>

// Mcd
#include <boost/hana/ext/boost/mpl/integral_c/constant.hpp>

#include <boost/mpl/integral_c.hpp>


namespace boost { namespace hana {
    template <>
    struct IntegralConstant::instance<BoostMplIntegralConstant>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return boost::mpl::integral_c<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_INTEGRAL_CONSTANT_HPP
