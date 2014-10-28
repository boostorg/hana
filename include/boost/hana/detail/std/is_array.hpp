/*!
@file
Defines an equivalent to `std::is_array`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_ARRAY_HPP
#define BOOST_HANA_DETAIL_STD_IS_ARRAY_HPP

#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T>           struct is_array       : false_type { };
    template <typename T>           struct is_array<T[]>  : true_type { };
    template <typename T, size_t N> struct is_array<T[N]> : true_type { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_ARRAY_HPP
