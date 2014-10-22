/*!
@file
Defines `boost::hana::ext::std::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/fwd/ext/std/integral_constant.hpp>

// instances
#include <boost/hana/constant.hpp>
#include <boost/hana/integral_constant.hpp>

#include <type_traits>


namespace boost { namespace hana {
    template <>
    struct Constant::instance<ext::std::IntegralConstant> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <>
    struct IntegralConstant::instance<ext::std::IntegralConstant>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return ::std::integral_constant<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
