/*!
@file
Defines an equivalent to `std::declval`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_DECLVAL_HPP
#define BOOST_HANA_DETAIL_STD_DECLVAL_HPP

#include <boost/hana/detail/std/add_rvalue_reference.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T>
    typename add_rvalue_reference<T>::type declval();
}}}}

#endif // !BOOST_HANA_DETAIL_STD_DECLVAL_HPP
