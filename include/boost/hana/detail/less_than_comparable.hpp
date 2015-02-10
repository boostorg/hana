/*!
@file
Defines `boost::hana::detail::concept::LessThanComparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LESS_THAN_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_LESS_THAN_COMPARABLE_HPP

#include <boost/hana/detail/std/common_type.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/void_t.hpp>


namespace boost { namespace hana { namespace detail { namespace concept {
    template <typename T, typename U = T, typename = void>
    struct LessThanComparable : detail::std::false_type { };

    template <typename T>
    struct LessThanComparable<T, T, detail::std::void_t<
        decltype(detail::std::declval<T>() < detail::std::declval<T>() ? 0:0)
    >> : detail::std::true_type { };

    template <typename T, typename U>
    struct LessThanComparable<T, U, detail::std::enable_if_t<
        !detail::std::is_same<T, U>{}, detail::std::void_t<
            decltype(detail::std::declval<T>() < detail::std::declval<U>() ? 0:0),
            decltype(detail::std::declval<U>() < detail::std::declval<T>() ? 0:0),
            typename detail::std::common_type<T, U>::type
    >>>
        : detail::std::integral_constant<bool,
            LessThanComparable<T>{} && LessThanComparable<U>{} &&
            LessThanComparable<typename detail::std::common_type<T, U>::type>{}
        >
    { };
}}}} // end namespace boost::hana::detail::concept

#endif // !BOOST_HANA_DETAIL_LESS_THAN_COMPARABLE_HPP
