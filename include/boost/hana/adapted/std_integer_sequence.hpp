/*!
@file
Adapts `std::integer_sequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ADAPTED_STD_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_ADAPTED_STD_INTEGER_SEQUENCE_HPP

#include <boost/hana/adapted/std_integral_constant.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/iterable.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    struct StdIntegerSequence;

    template <typename T, T ...v>
    struct datatype<std::integer_sequence<T, v...>> {
        using type = StdIntegerSequence;
    };

    template <>
    struct Iterable::instance<StdIntegerSequence> : Iterable::mcd {
        template <typename T, T x, T ...xs>
        static constexpr auto head_impl(std::integer_sequence<T, x, xs...>)
        { return std::integral_constant<T, x>{}; }

        template <typename T, T x, T ...xs>
        static constexpr auto tail_impl(std::integer_sequence<T, x, xs...>)
        { return std::integer_sequence<T, xs...>{}; }

        template <typename T, T ...xs>
        static constexpr auto is_empty_impl(std::integer_sequence<T, xs...>)
        { return bool_<sizeof...(xs) == 0>; }
    };

    template <>
    struct Comparable::instance<StdIntegerSequence, StdIntegerSequence>
        : Iterable::ComparableInstance
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ADAPTED_STD_INTEGER_SEQUENCE_HPP
