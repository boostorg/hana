/*!
@file
Defines an equivalent to `std::conditional`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_CONDITIONAL_HPP
#define BOOST_HANA_DETAIL_STD_CONDITIONAL_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    template <bool, typename Then, typename Else>
    struct conditional { using type = Then; };

    template <typename Then, typename Else>
    struct conditional<false, Then, Else> { using type = Else; };

    template <bool Cond, typename Then, typename Else>
    using conditional_t = typename conditional<Cond, Then, Else>::type;
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_CONDITIONAL_HPP
