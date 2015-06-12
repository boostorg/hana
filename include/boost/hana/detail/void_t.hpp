/*!
@file
Defines an equivalent to the proposed `std::void_t`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VOID_T_HPP
#define BOOST_HANA_DETAIL_VOID_T_HPP

namespace boost { namespace hana { namespace detail {
    template <typename ...>
    using void_t = void;
}}}

#endif // !BOOST_HANA_DETAIL_VOID_T_HPP
