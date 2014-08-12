/*!
@file
Defines the instance of `boost::hana::List` for `std::array`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_LIST_HPP
#define BOOST_HANA_EXT_STD_ARRAY_LIST_HPP

#include <boost/hana/ext/std/array/foldable.hpp>
#include <boost/hana/ext/std/array/iterable.hpp>
#include <boost/hana/ext/std/array/monad.hpp>
#include <boost/hana/list/mcd.hpp>

#include <array>
#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    template <>
    struct List::instance<StdArray> : List::mcd<StdArray> {
        struct anything { };

        static constexpr auto nil_impl()
        { return std::array<anything, 0>{}; }


        template <typename X, typename T, std::size_t N, std::size_t ...index>
        static constexpr auto cons_helper(
            X x, std::array<T, N> arr, std::index_sequence<index...>
        )
        { return std::array<T, N + 1>{{x, arr[index]...}}; }

        template <typename X, typename T, std::size_t N>
        static constexpr auto cons_impl(X x, std::array<T, N> arr)
        { return cons_helper(x, arr, std::make_index_sequence<N>{}); }

        template <typename X>
        static constexpr auto cons_impl(X x, std::array<anything, 0>)
        { return cons_impl(x, std::array<X, 0>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_LIST_HPP
