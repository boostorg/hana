/*!
 * @file
 * Adapts `std::tuple`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STD_TUPLE_HPP
#define BOOST_HANA_STD_TUPLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    template <typename ...Xs>
    struct Iterable<std::tuple<Xs...>> : defaults<Iterable> {
        static constexpr auto head_impl(std::tuple<Xs...> tuple)
        { return std::get<0>(tuple); }

        template <std::size_t ...Index>
        static constexpr auto
        tail_helper(std::tuple<Xs...> tuple, std::index_sequence<Index...>)
        { return std::make_tuple(std::get<Index + 1>(tuple)...); }

        static constexpr auto tail_impl(std::tuple<Xs...> tuple)
        { return tail_helper(tuple, std::make_index_sequence<sizeof...(Xs) - 1>{}); }

        static constexpr auto is_empty_impl(std::tuple<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <typename ...Xs>
    struct Functor<std::tuple<Xs...>> {
        template <typename F, std::size_t ...Index>
        static constexpr auto
        helper(F f, std::tuple<Xs...> tuple, std::index_sequence<Index...>)
        { return std::make_tuple(f(std::get<Index>(tuple))...); }

        template <typename F>
        static constexpr auto fmap_impl(F f, std::tuple<Xs...> tuple)
        { return helper(f, tuple, std::index_sequence_for<Xs...>{}); }
    };

    template <typename ...Xs>
    struct Foldable<std::tuple<Xs...>>
        : detail::foldable_from_iterable
    { };

    template <typename ...Xs, typename ...Ys>
    struct Comparable<std::tuple<Xs...>, std::tuple<Ys...>>
        : detail::comparable_from_iterable
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STD_TUPLE_HPP
