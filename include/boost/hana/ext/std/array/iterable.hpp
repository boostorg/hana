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
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/ext/std/array/array.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <array>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<StdArray> : Iterable::mcd {
        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs)
        { return detail::std::forward<Xs>(xs)[0]; }

        template <typename T, detail::std::size_t N, typename Xs, detail::std::size_t ...index>
        static constexpr auto tail_helper(Xs&& xs, detail::std::index_sequence<index...>) {
            return std::array<T, N - 1>{{
                detail::std::forward<Xs>(xs)[index + 1]...
            }};
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            using RawArray = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = std::tuple_size<RawArray>::value;
            using T = typename RawArray::value_type;
            return tail_helper<T, N>(
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<N - 1>{}
            );
        }

        template <typename T, detail::std::size_t N>
        static constexpr auto is_empty_impl(std::array<T, N> const&)
        { return bool_<N == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_ITERABLE_HPP
