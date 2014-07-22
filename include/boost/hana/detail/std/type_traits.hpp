/*!
@file
Defines equivalents for some `std` type traits to remove dependencies on
the standard library.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_TYPE_TRAITS_HPP
#define BOOST_HANA_DETAIL_STD_TYPE_TRAITS_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    // integral_constant
    template <typename T, T v>
    struct integral_constant {
        using value_type = T;
        static constexpr value_type value = v;
        using type = integral_constant;
        constexpr operator value_type() const { return value; }
        constexpr value_type operator()() const { return value; }
    };


    // enable_if
    template <bool Cond, typename T = void>
    struct enable_if { using type = T; };

    template <typename T>
    struct enable_if<false, T> { };

    template <bool Cond, typename T = void>
    using enable_if_t = typename enable_if<Cond, T>::type;


    // remove_cv
    template <typename T> struct remove_cv { using type = T; };
    template <typename T> struct remove_cv<T const> { using type = T; };
    template <typename T> struct remove_cv<T volatile> { using type = T; };
    template <typename T> struct remove_cv<T const volatile> { using type = T; };
    template <typename T> using remove_cv_t = typename remove_cv<T>::type;


    // is_same
    template <typename T, typename U>
    struct is_same : integral_constant<bool, false> { };

    template <typename T>
    struct is_same<T, T> : integral_constant<bool, true> { };


    // is_integral
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

#endif // !BOOST_HANA_DETAIL_STD_TYPE_TRAITS_HPP
