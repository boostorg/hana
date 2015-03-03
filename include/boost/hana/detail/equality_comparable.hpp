/*!
@file
Defines `boost::hana::detail::concept::EqualityComparable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_EQUALITY_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_EQUALITY_COMPARABLE_HPP

#include <boost/hana/detail/std/common_type.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/void_t.hpp>


namespace boost { namespace hana { namespace detail { namespace concept {
    template <typename T, typename U = T, typename = void>
    struct EqualityComparable : detail::std::false_type { };

    template <typename T>
    struct EqualityComparable<T, T, detail::std::void_t<
        decltype(detail::std::declval<T>() == detail::std::declval<T>() ? 0:0),
        decltype(detail::std::declval<T>() != detail::std::declval<T>() ? 0:0)
    >> : detail::std::true_type { };

    template <typename T, typename U>
    struct EqualityComparable<T, U, detail::std::enable_if_t<
        !detail::std::is_same<T, U>{}, detail::std::void_t<
            decltype(detail::std::declval<T>() == detail::std::declval<U>() ? 0:0),
            decltype(detail::std::declval<U>() == detail::std::declval<T>() ? 0:0),
            decltype(detail::std::declval<T>() != detail::std::declval<U>() ? 0:0),
            decltype(detail::std::declval<U>() != detail::std::declval<T>() ? 0:0),
            typename detail::std::common_type<T, U>::type
    >>> : detail::std::integral_constant<bool,
        EqualityComparable<T>{} && EqualityComparable<U>{} &&
        EqualityComparable<typename detail::std::common_type<T, U>::type>{}
    > { };
}}}} // end namespace boost::hana::detail::concept

#endif // !BOOST_HANA_DETAIL_EQUALITY_COMPARABLE_HPP
