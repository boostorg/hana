/*!
@file
Defines an equivalent to `std::is_rvalue_reference`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_RVALUE_REFERENCE_HPP
#define BOOST_HANA_DETAIL_STD_IS_RVALUE_REFERENCE_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T>
    struct is_rvalue_reference
        : false_type
    { };

    template <typename T>
    struct is_rvalue_reference<T&&>
        : true_type
    { };
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_RVALUE_REFERENCE_HPP
