/*!
@file
Forward declares `boost::hana::StdIntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_FWD_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_FWD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>

#include <type_traits>


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
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_FWD_HPP
