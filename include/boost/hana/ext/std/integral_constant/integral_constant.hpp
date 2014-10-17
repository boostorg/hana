/*!
@file
Defines the instance of `boost::hana::IntegralConstant` for
`boost::hana::StdIntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP

#include <boost/hana/ext/std/integral_constant/fwd.hpp>
#include <boost/hana/integral_constant/mcd.hpp>

// Mcd
#include <boost/hana/ext/std/integral_constant/constant.hpp>

#include <type_traits>


namespace boost { namespace hana {
    template <>
    struct IntegralConstant::instance<StdIntegralConstant>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return std::integral_constant<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP
