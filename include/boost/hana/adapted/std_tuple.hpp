/*!
@file
Adapts `std::tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ADAPTED_STD_TUPLE_HPP
#define BOOST_HANA_ADAPTED_STD_TUPLE_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/monad.hpp>

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
    struct Iterable::instance<StdTuple> : Iterable::mcd {
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
    struct List::instance<StdTuple> : List::mcd<StdTuple> {
        static constexpr auto nil_impl()
        { return std::tuple<>{}; }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return std::tuple_cat(std::make_tuple(x), xs); }
    };

    template <>
    struct Functor::instance<StdTuple> : Functor::fmap_mcd {
        template <typename F, typename ...Xs, std::size_t ...Index>
        static constexpr auto
        helper(F f, std::tuple<Xs...> tuple, std::index_sequence<Index...>)
        { return std::make_tuple(f(std::get<Index>(tuple))...); }

        template <typename F, typename ...Xs>
        static constexpr auto fmap_impl(F f, std::tuple<Xs...> tuple)
        { return helper(f, tuple, std::index_sequence_for<Xs...>{}); }
    };

    //! @todo Check `Applicative` laws.
    template <>
    struct Applicative::instance<StdTuple> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x)
        { return std::tuple<X>{x}; }

        template <typename Fs, typename Xs>
        static constexpr auto ap_impl(Fs fs, Xs xs)
        { return bind(fs, [=](auto f) { return fmap(f, xs); }); }
    };

    template <>
    struct Monad::instance<StdTuple> : Monad::flatten_mcd {
        template <typename ...Tuples, std::size_t ...Index>
        static constexpr auto
        helper(std::tuple<Tuples...> tuples, std::index_sequence<Index...>)
        { return std::tuple_cat(std::get<Index>(tuples)...); }

        template <typename ...Tuples>
        static constexpr auto flatten_impl(std::tuple<Tuples...> tuples)
        { return helper(tuples, std::index_sequence_for<Tuples...>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ADAPTED_STD_TUPLE_HPP
