/*!
@file
Defines concepts from the Standard library.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CONCEPTS_HPP
#define BOOST_HANA_DETAIL_CONCEPTS_HPP

#include <boost/hana/detail/std/common_type.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/void_t.hpp>


namespace boost { namespace hana { namespace detail {
    //////////////////////////////////////////////////////////////////////////
    // EqualityComparable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U = T, typename = void>
    struct EqualityComparable : detail::std::false_type { };

    template <typename T>
    struct EqualityComparable<T, T, detail::std::void_t<
        decltype(static_cast<T&&>(*(T*)0) == static_cast<T&&>(*(T*)0) ? 0:0),
        decltype(static_cast<T&&>(*(T*)0) != static_cast<T&&>(*(T*)0) ? 0:0)
    >> : detail::std::true_type { };

    template <typename T, typename U>
    struct EqualityComparable<T, U, detail::std::enable_if_t<
        !detail::std::is_same<T, U>{}, detail::std::void_t<
            decltype(static_cast<T&&>(*(T*)0) == static_cast<U&&>(*(U*)0) ? 0:0),
            decltype(static_cast<U&&>(*(U*)0) == static_cast<T&&>(*(T*)0) ? 0:0),
            decltype(static_cast<T&&>(*(T*)0) != static_cast<U&&>(*(U*)0) ? 0:0),
            decltype(static_cast<U&&>(*(U*)0) != static_cast<T&&>(*(T*)0) ? 0:0),
            typename detail::std::common_type<T, U>::type
    >>> : detail::std::integral_constant<bool,
        EqualityComparable<T>{}() &&
        EqualityComparable<U>{}() &&
        EqualityComparable<typename detail::std::common_type<T, U>::type>{}()
    > { };


    //////////////////////////////////////////////////////////////////////////
    // LessThanComparable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U = T, typename = void>
    struct LessThanComparable : detail::std::false_type { };

    template <typename T>
    struct LessThanComparable<T, T, detail::std::void_t<
        decltype(static_cast<T&&>(*(T*)0) < static_cast<T&&>(*(T*)0) ? 0:0)
    >> : detail::std::true_type { };

    template <typename T, typename U>
    struct LessThanComparable<T, U, detail::std::enable_if_t<
        !detail::std::is_same<T, U>{}(),
        detail::std::void_t<
            decltype(static_cast<T&&>(*(T*)0) < static_cast<U&&>(*(U*)0) ? 0:0),
            decltype(static_cast<U&&>(*(U*)0) < static_cast<T&&>(*(T*)0) ? 0:0),
            typename detail::std::common_type<T, U>::type
        >
    >>
        : detail::std::integral_constant<bool,
            LessThanComparable<T>{}() &&
            LessThanComparable<U>{}() &&
            LessThanComparable<typename detail::std::common_type<T, U>::type>{}()
        >
    { };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CONCEPTS_HPP
