/*!
@file
Adapts `std::array`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_HPP
#define BOOST_HANA_EXT_STD_ARRAY_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list.hpp>
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
    struct Iterable::instance<StdArray> : Iterable::mcd {
        template <typename T, std::size_t N>
        static constexpr T head_impl(std::array<T, N> arr)
        { return arr[0]; }

        //! @todo
        //! We needlessly create a ton of `size_t<indices>...` when we
        //! unpack the range. We should be able to unpack the range and
        //! get straight `std::size_t`s instead of `size_t<...>`s when
        //! we don't need them.
        template <typename T, std::size_t N>
        static constexpr auto tail_impl(std::array<T, N> arr)  {
            auto make_array = [=](auto ...indices) -> std::array<T, N - 1>
            { return {{arr[indices]...}}; };
            return unpack(make_array, range(size_t<1>, size_t<N>));
        }

        template <typename T, std::size_t N>
        static constexpr auto is_empty_impl(std::array<T, N> arr)
        { return bool_<N == 0>; }
    };

    template <>
    struct List::instance<StdArray> : List::mcd<StdArray> {
        struct anything { };

        static constexpr auto nil_impl() {
            return std::array<anything, 0>{};
        }

        template <typename X, typename T, std::size_t N>
        static constexpr auto cons_impl(X x, std::array<T, N> arr) {
            auto make_array = [=](auto ...indices) -> std::array<T, N + 1>
            { return {{x, arr[indices]...}}; };
            return unpack(make_array, range(size_t<0>, size_t<N>));
        }

        template <typename X>
        static constexpr auto cons_impl(X x, std::array<anything, 0>) {
            return cons_impl(x, std::array<X, 0>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_HPP
