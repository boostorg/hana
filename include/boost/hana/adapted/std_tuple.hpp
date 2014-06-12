/*!
@file
Adapts `std::tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STD_TUPLE_HPP
#define BOOST_HANA_STD_TUPLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    struct StdTuple;

    template <typename ...Xs>
    struct datatype<std::tuple<Xs...>> {
        using type = StdTuple;
    };

    template <>
    struct Iterable<StdTuple> : defaults<Iterable> {
        template <typename ...Xs>
        static constexpr auto head_impl(std::tuple<Xs...> tuple)
        { return std::get<0>(tuple); }

        template <typename ...Xs, std::size_t ...Index>
        static constexpr auto
        tail_helper(std::tuple<Xs...> tuple, std::index_sequence<Index...>)
        { return std::make_tuple(std::get<Index + 1>(tuple)...); }

        template <typename ...Xs>
        static constexpr auto tail_impl(std::tuple<Xs...> tuple)
        { return tail_helper(tuple, std::make_index_sequence<sizeof...(Xs) - 1>{}); }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(std::tuple<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <>
    struct Functor<StdTuple> : defaults<Functor>::with<StdTuple> {
        template <typename F, typename ...Xs, std::size_t ...Index>
        static constexpr auto
        helper(F f, std::tuple<Xs...> tuple, std::index_sequence<Index...>)
        { return std::make_tuple(f(std::get<Index>(tuple))...); }

        template <typename F, typename ...Xs>
        static constexpr auto fmap_impl(F f, std::tuple<Xs...> tuple)
        { return helper(f, tuple, std::index_sequence_for<Xs...>{}); }
    };

    template <>
    constexpr bool foldable_from_iterable<StdTuple> = true;

    template <>
    constexpr bool comparable_from_iterable<StdTuple> = true;
}} // end namespace boost::hana

#endif // !BOOST_HANA_STD_TUPLE_HPP
