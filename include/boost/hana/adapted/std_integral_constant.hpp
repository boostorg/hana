/*!
@file
Adapts `std::integral_constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_STD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>


namespace boost { namespace hana {
    using StdIntegralConstant = Integral;

    template <typename T, T v>
    struct datatype<std::integral_constant<T, v>> {
        using type = StdIntegralConstant;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STD_INTEGRAL_CONSTANT_HPP
