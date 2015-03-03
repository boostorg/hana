/*!
@file
Defines an equivalent to `std::move`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_MOVE_HPP
#define BOOST_HANA_DETAIL_STD_MOVE_HPP

#include <boost/hana/detail/std/remove_reference.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T>
    constexpr typename remove_reference<T>::type&& move(T&& t) noexcept {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_MOVE_HPP
