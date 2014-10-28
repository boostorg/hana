/*!
@file
Defines an equivalent to `std::decay`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_DECAY_HPP
#define BOOST_HANA_DETAIL_STD_DECAY_HPP

#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/is_array.hpp>
#include <boost/hana/detail/std/is_function.hpp>
#include <boost/hana/detail/std/remove_cv.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T,
        typename U = typename remove_reference<T>::type,
        bool = is_array<U>::value || is_function<U>::value>
    struct decay;

    template <typename T> T byval(T);

    template <typename T, typename U>
    struct decay<T, U, true> {
        using type = decltype(byval(declval<U>()));
    };

    template <typename T, typename U>
    struct decay<T, U, false> : remove_cv<U> { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_DECAY_HPP
