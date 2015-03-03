/*!
@file
Forward declares `boost::hana::ext::std::IntegralConstant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_FWD_EXT_STD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace ext { namespace std {
        template <typename T>
        struct IntegralConstant { using value_type = T; };
    }}

    namespace std_ic_detail {
        template <typename T, T v>
        constexpr bool
        is_std_integral_constant(::std::integral_constant<T, v>*)
        { return true; }

        constexpr bool is_std_integral_constant(...)
        { return false; }
    }

    template <typename T>
    struct datatype<T, when<std_ic_detail::is_std_integral_constant((T*)0)>> {
        using type = ext::std::IntegralConstant<
            typename datatype<typename T::value_type>::type
        >;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_STD_INTEGRAL_CONSTANT_HPP
