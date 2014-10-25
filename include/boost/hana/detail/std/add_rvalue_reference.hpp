/*!
@file
Defines an equivalent to `std::add_rvalue_reference`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_ADD_RVALUE_REFERENCE_HPP
#define BOOST_HANA_DETAIL_STD_ADD_RVALUE_REFERENCE_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T> struct add_rvalue_reference            { using type = T&&; };
    template <> struct add_rvalue_reference<void>                { using type = void; };
    template <> struct add_rvalue_reference<void const>          { using type = void const; };
    template <> struct add_rvalue_reference<void volatile>       { using type = void volatile; };
    template <> struct add_rvalue_reference<void const volatile> { using type = void const volatile; };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_ADD_RVALUE_REFERENCE_HPP
