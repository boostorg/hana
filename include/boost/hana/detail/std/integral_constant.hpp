/*!
@file
Defines an equivalent to `std::integral_constant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_DETAIL_STD_INTEGRAL_CONSTANT_HPP

namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T, T v>
    struct integral_constant {
        using value_type = T;
        static constexpr value_type value = v;
        using type = integral_constant;
        constexpr operator value_type() const { return value; }
        constexpr value_type operator()() const { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_INTEGRAL_CONSTANT_HPP
