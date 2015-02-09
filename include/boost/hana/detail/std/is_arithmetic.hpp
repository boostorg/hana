/*!
@file
Defines an equivalent to `std::is_arithmetic`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_ARITHMETIC_HPP
#define BOOST_HANA_DETAIL_STD_IS_ARITHMETIC_HPP

#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_floating_point.hpp>
#include <boost/hana/detail/std/is_integral.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T>
    struct is_arithmetic
        : integral_constant<bool,
            is_integral<T>::value ||
            is_floating_point<T>::value
        >
    { };

    template <typename T>
    struct is_non_boolean_arithmetic
        : is_arithmetic<T>
    { };

    template <>
    struct is_non_boolean_arithmetic<bool>
        : false_type
    { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_ARITHMETIC_HPP
