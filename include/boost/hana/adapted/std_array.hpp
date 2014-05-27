/*!
 * @file
 * Adapts `std::array`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STD_ARRAY_HPP
#define BOOST_HANA_STD_ARRAY_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/range.hpp>

#include <array>
#include <cstddef>


namespace boost { namespace hana {
    struct StdArray;

    template <typename T, std::size_t N>
    struct datatype<std::array<T, N>> {
        using type = StdArray;
    };

    template <>
    struct Iterable<StdArray> : defaults<Iterable> {
        template <typename T, std::size_t N>
        static constexpr T head_impl(std::array<T, N> arr)
        { return arr[0]; }

        template <typename T, std::size_t N, std::size_t ...Index>
        static constexpr std::array<T, sizeof...(Index)>
        tail_helper(std::array<T, N> arr, Range<std::size_t, Index...>)
        { return {{arr[Index]...}}; }

        template <typename T, std::size_t N>
        static constexpr auto tail_impl(std::array<T, N> arr)
        { return tail_helper(arr, range(size_t<1>, size_t<N>)); }

        template <typename T, std::size_t N>
        static constexpr Bool<N == 0> is_empty_impl(std::array<T, N> arr)
        { return {}; }
    };

    template <>
    struct Foldable<StdArray>
        : detail::foldable_from_iterable
    { };

    template <>
    struct Comparable<StdArray, StdArray>
        : detail::comparable_from_iterable
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STD_ARRAY_HPP
