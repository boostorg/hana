/*!
@file
Adapts `std::integral_constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>


namespace boost { namespace hana {
    using StdIntegralConstant = Integral;

    namespace adapted_detail {
        template <typename T, T v>
        constexpr bool
        is_std_integral_constant(std::integral_constant<T, v>*)
        { return true; }

        constexpr bool is_std_integral_constant(...)
        { return false; }
    }

    template <typename T>
    struct datatype<T,
        std::enable_if_t<adapted_detail::is_std_integral_constant((T*)0)>
    > {
        using type = StdIntegralConstant;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
