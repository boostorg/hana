/*!
@file
Defines an equivalent to `std::size_t` to remove dependencies on `<cstddef>`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_SIZE_T_HPP
#define BOOST_HANA_DETAIL_STD_SIZE_T_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    using size_t = decltype(sizeof(int));
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_SIZE_T_HPP
