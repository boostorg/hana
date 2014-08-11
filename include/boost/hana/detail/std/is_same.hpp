/*!
@file
Defines an equivalent to `std::is_same`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_SAME_HPP
#define BOOST_HANA_DETAIL_STD_IS_SAME_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T, typename U>
    struct is_same : integral_constant<bool, false> { };

    template <typename T>
    struct is_same<T, T> : integral_constant<bool, true> { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_SAME_HPP
