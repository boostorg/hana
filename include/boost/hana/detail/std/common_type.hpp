/*!
@file
Defines an equivalent to `std::common_type`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_COMMON_TYPE_HPP
#define BOOST_HANA_DETAIL_STD_COMMON_TYPE_HPP

#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/declval.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T, typename U, typename = void>
    struct common_type { };

    template <typename T, typename U>
    struct common_type<T, U, decltype((void)(true ? declval<T>() : declval<U>()))> {
        using type = typename decay<
            decltype(true ? declval<T>() : declval<U>())
        >::type;
    };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_COMMON_TYPE_HPP
