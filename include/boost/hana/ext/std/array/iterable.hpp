/*!
@file
Defines the instance of `boost::hana::Iterable` for `std::array`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_ITERABLE_HPP
#define BOOST_HANA_EXT_STD_ARRAY_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/ext/std/array/array.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <array>
#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<StdArray> : Iterable::mcd {
        template <typename T, std::size_t N>
        static constexpr T head_impl(std::array<T, N> arr)
        { return arr[0]; }

        template <typename T, std::size_t N, std::size_t ...index>
        static constexpr auto tail_helper(
            std::array<T, N> arr, std::index_sequence<index...>
        )
        { return std::array<T, N - 1>{{arr[index + 1]...}}; }

        template <typename T, std::size_t N>
        static constexpr auto tail_impl(std::array<T, N> arr)
        { return tail_helper(arr, std::make_index_sequence<N-1>{}); }

        template <typename T, std::size_t N>
        static constexpr auto is_empty_impl(std::array<T, N> arr)
        { return bool_<N == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_ITERABLE_HPP
