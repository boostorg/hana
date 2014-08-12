/*!
@file
Defines the instance of `boost::hana::Iterable` for `std::integer_sequence`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_ITERABLE_HPP
#define BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/ext/std/integer_sequence/integer_sequence.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_ITERABLE_HPP
