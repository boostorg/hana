/*!
@file
Defines an equivalent to `std::remove_reference`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_REMOVE_REFERENCE_HPP
#define BOOST_HANA_DETAIL_STD_REMOVE_REFERENCE_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T> struct remove_reference       { using type = T; };
    template <typename T> struct remove_reference<T&>   { using type = T; };
    template <typename T> struct remove_reference<T&&>  { using type = T; };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_REMOVE_REFERENCE_HPP
