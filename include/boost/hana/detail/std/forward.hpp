/*!
@file
Defines an equivalent to `std::forward`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_FORWARD_HPP
#define BOOST_HANA_DETAIL_STD_FORWARD_HPP

#include <boost/hana/detail/std/is_lvalue_reference.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T>
    constexpr T&& forward(typename remove_reference<T>::type& t) noexcept {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept {
        static_assert(!is_lvalue_reference<T>::value,
        "an rvalue can't be forwarded as an lvalue");
        return static_cast<T&&>(t);
    }
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_FORWARD_HPP
