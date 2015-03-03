/*!
@file
Defines an equivalent to `std::is_integral`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_INTEGRAL_HPP
#define BOOST_HANA_DETAIL_STD_IS_INTEGRAL_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T> struct is_integral : false_type { };
    template <typename T> struct is_integral<T const> : is_integral<T> { };
    template <typename T> struct is_integral<T volatile> : is_integral<T> { };
    template <typename T> struct is_integral<T const volatile> : is_integral<T> { };
    template <> struct is_integral<bool>               : true_type { };
    template <> struct is_integral<char>               : true_type { };
    template <> struct is_integral<signed char>        : true_type { };
    template <> struct is_integral<unsigned char>      : true_type { };
    template <> struct is_integral<wchar_t>            : true_type { };
    template <> struct is_integral<char16_t>           : true_type { };
    template <> struct is_integral<char32_t>           : true_type { };
    template <> struct is_integral<short>              : true_type { };
    template <> struct is_integral<unsigned short>     : true_type { };
    template <> struct is_integral<int>                : true_type { };
    template <> struct is_integral<unsigned int>       : true_type { };
    template <> struct is_integral<long>               : true_type { };
    template <> struct is_integral<unsigned long>      : true_type { };
    template <> struct is_integral<long long>          : true_type { };
    template <> struct is_integral<unsigned long long> : true_type { };


    template <typename T>
    struct is_non_boolean_integral : is_integral<T> { };
    template <>
    struct is_non_boolean_integral<bool> : false_type { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_INTEGRAL_HPP
