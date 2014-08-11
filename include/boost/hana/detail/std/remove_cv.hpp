/*!
@file
Defines an equivalent to `std::remove_cv`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_REMOVE_CV_HPP
#define BOOST_HANA_DETAIL_STD_REMOVE_CV_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T> struct remove_cv { using type = T; };
    template <typename T> struct remove_cv<T const> { using type = T; };
    template <typename T> struct remove_cv<T volatile> { using type = T; };
    template <typename T> struct remove_cv<T const volatile> { using type = T; };
    template <typename T> using remove_cv_t = typename remove_cv<T>::type;
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_REMOVE_CV_HPP
