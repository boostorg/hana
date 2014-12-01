/*!
@file
Defines an equivalent to `std::is_floating_point`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_FLOATING_POINT_HPP
#define BOOST_HANA_DETAIL_STD_IS_FLOATING_POINT_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T> struct is_floating_point : false_type { };
    template <typename T> struct is_floating_point<T const> : is_floating_point<T> { };
    template <typename T> struct is_floating_point<T volatile> : is_floating_point<T> { };
    template <typename T> struct is_floating_point<T const volatile> : is_floating_point<T> { };
    template <> struct is_floating_point<float> : true_type { };
    template <> struct is_floating_point<double> : true_type { };
    template <> struct is_floating_point<long double> : true_type { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_FLOATING_POINT_HPP
