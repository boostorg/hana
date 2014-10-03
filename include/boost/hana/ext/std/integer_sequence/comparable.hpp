/*!
@file
Defines the instance of `boost::hana::Comparable` for `std::integer_sequence`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_COMPARABLE_HPP
#define BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/std/integer_sequence/integer_sequence.hpp>

#include <utility>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<StdIntegerSequence, StdIntegerSequence>
        : Comparable::equal_mcd
    {
        template <typename X, X ...xs, typename Y, Y ...ys>
        static constexpr auto equal_impl(
            std::integer_sequence<X, xs...>,
            std::integer_sequence<Y, ys...>,
            // this dummy parameter disables this specialization if
            // sizeof...(xs) != sizeof...(ys)
            char(*)[sizeof...(xs) == sizeof...(ys)] = 0)
        {
            return bool_<detail::std::is_same<
                std::integer_sequence<bool, (xs == ys)...>,
                std::integer_sequence<bool, (xs, true)...>
            >::value>;
        }

        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys, ...)
        { return false_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_COMPARABLE_HPP
