/*!
@file
Defines an equivalent to `std::is_integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_INTEGRAL_HPP
#define BOOST_HANA_DETAIL_STD_IS_INTEGRAL_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T> struct is_integral : integral_constant<bool, false> { };
    template <typename T> struct is_integral<T const> : is_integral<T> { };
    template <typename T> struct is_integral<T volatile> : is_integral<T> { };
    template <typename T> struct is_integral<T const volatile> : is_integral<T> { };
    template <> struct is_integral<bool>               : integral_constant<bool, true> { };
    template <> struct is_integral<char>               : integral_constant<bool, true> { };
    template <> struct is_integral<signed char>        : integral_constant<bool, true> { };
    template <> struct is_integral<unsigned char>      : integral_constant<bool, true> { };
    template <> struct is_integral<wchar_t>            : integral_constant<bool, true> { };
    template <> struct is_integral<char16_t>           : integral_constant<bool, true> { };
    template <> struct is_integral<char32_t>           : integral_constant<bool, true> { };
    template <> struct is_integral<short>              : integral_constant<bool, true> { };
    template <> struct is_integral<unsigned short>     : integral_constant<bool, true> { };
    template <> struct is_integral<int>                : integral_constant<bool, true> { };
    template <> struct is_integral<unsigned int>       : integral_constant<bool, true> { };
    template <> struct is_integral<long>               : integral_constant<bool, true> { };
    template <> struct is_integral<unsigned long>      : integral_constant<bool, true> { };
    template <> struct is_integral<long long>          : integral_constant<bool, true> { };
    template <> struct is_integral<unsigned long long> : integral_constant<bool, true> { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_INTEGRAL_HPP
