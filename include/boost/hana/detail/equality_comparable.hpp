/*!
@file
Defines `boost::hana::detail::concept::EqualityComparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_EQUALITY_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_EQUALITY_COMPARABLE_HPP

#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace concept {
    template <typename T, typename = void>
    struct EqualityComparable : detail::std::false_type { };

    template <typename T>
    struct EqualityComparable<T, decltype(
        static_cast<bool>(detail::std::declval<T>() == detail::std::declval<T>()),
        static_cast<bool>(detail::std::declval<T>() != detail::std::declval<T>()),
        void()
    )>
        : detail::std::true_type
    { };
}}}} // end namespace boost::hana::detail::concept

#endif // !BOOST_HANA_DETAIL_EQUALITY_COMPARABLE_HPP
