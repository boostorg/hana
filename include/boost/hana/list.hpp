/*!
 * @file
 * Defines `boost::hana::List`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_HPP
#define BOOST_HANA_LIST_HPP

#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    //! @ingroup datatypes
    template <typename ...Xs>
    struct List {
        std::tuple<Xs...> storage_;
    };

    constexpr struct _list {
        template <typename ...Xs>
        constexpr List<Xs...> operator()(Xs... xs) const
        { return {{xs...}}; }
    } list{};

    template <typename ...Xs>
    struct Iterable<List<Xs...>> {
        static constexpr auto head_impl(List<Xs...> xs)
        { return std::get<0>(xs.storage_); }


        template <std::size_t ...Index>
        static constexpr auto
        helper(List<Xs...> xs, std::index_sequence<Index...>)
        { return list(std::get<Index + 1>(xs.storage_)...); }

        static constexpr auto tail_impl(List<Xs...> xs)
        { return helper(xs, std::make_index_sequence<sizeof...(Xs) - 1>{}); }


        static constexpr Bool<sizeof...(Xs) == 0> is_empty_impl(List<Xs...>)
        { return {}; }
    };

    template <typename ...Xs>
    struct Functor<List<Xs...>> {
        template <typename F, std::size_t ...Index>
        static constexpr auto
        helper(F f, List<Xs...> xs, std::index_sequence<Index...>)
        { return list(f(std::get<Index>(xs.storage_))...); }

        template <typename F>
        static constexpr auto fmap_impl(F f, List<Xs...> xs)
        { return helper(f, xs, std::index_sequence_for<Xs...>{}); }
    };

    template <typename X, typename ...Xs>
    struct Foldable<List<X, Xs...>> {
        template <typename F, typename State>
        static constexpr auto foldl_impl(F f, State s, List<X, Xs...> xs)
        { return foldl(f, f(s, head(xs)), tail(xs)); }

        template <typename F, typename State>
        static constexpr auto foldr_impl(F f, State s, List<X, Xs...> xs)
        { return f(head(xs), foldr(f, s, tail(xs))); }
    };

    template <>
    struct Foldable<List<>> {
        template <typename F, typename State>
        static constexpr auto foldl_impl(F f, State s, List<> xs)
        { return s; }

        template <typename F, typename State>
        static constexpr auto foldr_impl(F f, State s, List<> xs)
        { return s; }
    };

    // comparison
    template <typename ...Xs>
    constexpr Bool<false> operator==(List<Xs...>, List<>) { return {}; }

    template <typename ...Ys>
    constexpr Bool<false> operator==(List<>, List<Ys...>) { return {}; }

    constexpr Bool<true> operator==(List<>, List<>) { return {}; }

    template <typename X, typename ...Xs, typename Y, typename ...Ys>
    constexpr auto operator==(List<X, Xs...> xs, List<Y, Ys...> ys)
        // decltype is required or Clang says
        // error: invalid argument type 'auto' to unary expression
        -> decltype(head(xs) == head(ys) && tail(xs) == tail(ys))
    { return head(xs) == head(ys) && tail(xs) == tail(ys); }

    template <typename ...Xs, typename ...Ys>
    constexpr auto operator!=(List<Xs...> xs, List<Ys...> ys)
        // decltype is required or Clang says
        // error: invalid argument type 'auto' to unary expression
        -> decltype(!(xs == ys))
    { return !(xs == ys); }
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
