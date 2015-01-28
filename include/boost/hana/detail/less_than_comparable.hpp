/*!
@file
Defines `boost::hana::detail::concept::LessThanComparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LESS_THAN_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_LESS_THAN_COMPARABLE_HPP

#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace concept {
    template <typename T, typename = void>
    struct LessThanComparable : detail::std::false_type { };

    template <typename T>
    struct LessThanComparable<T, decltype(
        detail::std::declval<T>() < detail::std::declval<T>() ? void() : void()
    )>
        : detail::std::true_type
    { };
}}}} // end namespace boost::hana::detail::concept

#endif // !BOOST_HANA_DETAIL_LESS_THAN_COMPARABLE_HPP
