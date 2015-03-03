/*!
@file
Defines an equivalent to `std::is_function`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_IS_FUNCTION_HPP
#define BOOST_HANA_DETAIL_STD_IS_FUNCTION_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename F>
    struct is_function : false_type { };

    template <typename R, typename ...A>
    struct is_function<R(A...)> : true_type {};

    template <typename R, typename ...A>
    struct is_function<R(A..., ...)> : true_type {};
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_IS_FUNCTION_HPP
