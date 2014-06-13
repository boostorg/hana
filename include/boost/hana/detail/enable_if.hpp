/*!
@file
Defines `boost::hana::detail::enable_if`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_ENABLE_IF_HPP
#define BOOST_HANA_DETAIL_ENABLE_IF_HPP

namespace boost { namespace hana { namespace detail {
    template <bool Cond, typename T = void>
    struct enable_if { using type = T; };

    template <typename T>
    struct enable_if<false, T> { };

    template <bool Cond, typename T = void>
    using enable_if_t = typename enable_if<Cond, T>::type;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_ENABLE_IF_HPP
