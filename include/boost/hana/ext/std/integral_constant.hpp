/*!
@file
Adapts `std::integral_constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/constant/mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/integral_constant/mcd.hpp>

#include <type_traits>

// Goodies
#include <boost/hana/comparable/integral_constant_mcd.hpp>
#include <boost/hana/orderable/integral_constant_mcd.hpp>
#include <boost/hana/logical/integral_constant_mcd.hpp>

#include <boost/hana/monoid/integral_constant_mcd.hpp>
#include <boost/hana/group/integral_constant_mcd.hpp>
#include <boost/hana/ring/integral_constant_mcd.hpp>
#include <boost/hana/integral_domain/integral_constant_mcd.hpp>


namespace boost { namespace hana {
    struct StdIntegralConstant;

    namespace adapted_detail {
        template <typename T, T v>
        constexpr bool
        is_std_integral_constant(std::integral_constant<T, v>*)
        { return true; }

        constexpr bool is_std_integral_constant(...)
        { return false; }
    }

    template <typename T>
    struct datatype<T, when<adapted_detail::is_std_integral_constant((T*)0)>> {
        using type = StdIntegralConstant;
    };

    template <>
    struct Constant::instance<StdIntegralConstant> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <>
    struct IntegralConstant::instance<StdIntegralConstant>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return std::integral_constant<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
