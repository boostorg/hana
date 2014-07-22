/*!
@file
Defines `boost::hana::detail::std::integer_sequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_DETAIL_STD_INTEGER_SEQUENCE_HPP

#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail { namespace std {
    template <typename T, T ...n>
    struct integer_sequence { };

    template <size_t ...n>
    using index_sequence = integer_sequence<size_t, n...>;


    namespace int_seq_detail {
        template <typename L, typename R>
        struct concat;

        template <typename T, T ...m, T ...n>
        struct concat<integer_sequence<T, m...>, integer_sequence<T, n...>> {
            using type = integer_sequence<T, m..., (sizeof...(m) + n)...>;
        };

        template <size_t n>
        struct make_index_sequence {
            using type = typename concat<
                typename make_index_sequence<n / 2>::type,
                typename make_index_sequence<n - n / 2>::type
            >::type;
        };

        template <>
        struct make_index_sequence<0> {
            using type = index_sequence<>;
        };

        template <>
        struct make_index_sequence<1> {
            using type = index_sequence<0>;
        };

        template <typename T, typename U>
        struct cast_to;

        template <typename T, typename U, U ...u>
        struct cast_to<T, integer_sequence<U, u...>> {
            using type = integer_sequence<T, static_cast<T>(u)...>;
        };
    } // end namespace int_seq_detail

    template <typename T, T n>
    using make_integer_sequence = typename int_seq_detail::cast_to<T,
        typename int_seq_detail::make_index_sequence<
            static_cast<size_t>(n)
        >::type
    >::type;

    template <size_t n>
    using make_index_sequence =
        typename int_seq_detail::make_index_sequence<n>::type;

    template <typename ...T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
}}}} // end namespace boost::hana::detail::std

#endif // !BOOST_HANA_DETAIL_STD_INTEGER_SEQUENCE_HPP
