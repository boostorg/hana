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

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! @todo How to implement iterate and repeat?
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
    constexpr auto list_t = list(type<Xs>...);

    template <typename T, T ...xs>
    constexpr auto list_c = list(Integral<T, xs>{}...);

    template <typename ...Xs>
    struct Iterable<List<Xs...>> : defaults<Iterable> {
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
    struct Functor<List<Xs...>> : defaults<Functor> {
        template <typename F, std::size_t ...Index>
        static constexpr auto
        helper(F f, List<Xs...> xs, std::index_sequence<Index...>)
        { return list(f(std::get<Index>(xs.storage_))...); }

        template <typename F>
        static constexpr auto fmap_impl(F f, List<Xs...> xs)
        { return helper(f, xs, std::index_sequence_for<Xs...>{}); }
    };

    template <typename ...Xs>
    struct Foldable<List<Xs...>>
        : detail::foldable_from_iterable
    { };

    template <typename ...Xs, typename ...Ys>
    struct Comparable<List<Xs...>, List<Ys...>>
        : detail::comparable_from_iterable
    { };

    template <typename ...Xs, typename ...Ys>
    constexpr auto operator==(List<Xs...> xs, List<Ys...> ys)
    {  return equal(xs, ys); }

    template <typename ...Xs, typename ...Ys>
    constexpr auto operator!=(List<Xs...> xs, List<Ys...> ys)
    {  return not_equal(xs, ys); }
}} // end namespace boost::hana

#include <boost/hana/range.hpp>

namespace boost { namespace hana {
    constexpr struct _zip_with {
        template <typename F, typename ...Lists>
        constexpr auto operator()(F f, Lists ...lists) const {
            return fmap(
                [=](auto index) { return f(at(index, lists)...); },
                range(size_t<0>, minimum(list(length(lists)...)))
            );
        }
    } zip_with{};

    constexpr struct _zip {
        template <typename ...Lists>
        constexpr auto operator()(Lists ...lists) const
        { return zip_with(list, lists...); }
    } zip{};

    constexpr struct _cons {
        template <typename X, typename ...Xs>
        constexpr List<X, Xs...> operator()(X x, List<Xs...> xs) const
        { return {std::tuple_cat(std::make_tuple(x), xs.storage_)}; }
    } cons{};

    constexpr struct _snoc {
        template <typename ...Xs, typename X>
        constexpr List<Xs..., X> operator()(List<Xs...> xs, X x) const
        { return {std::tuple_cat(xs.storage_, std::make_tuple(x))}; }
    } snoc{};

    constexpr struct _take {
        template <typename T, T t>
        static constexpr SizeT<t> to_size_t(Integral<T, t>)
        { return {}; }

        template <typename N, typename ...Xs>
        constexpr auto operator()(N n, List<Xs...> xs) const {
            auto min = [](auto a, auto b) { return if_(a < b, a, b); };
            return fmap(
                [=](auto index) { return at(index, xs); },
                range(size_t<0>, min(to_size_t(n), length(xs)))
            );
        }
    } take{};

    constexpr struct _take_while {
        template <typename Pred, typename ...Xs>
        constexpr auto operator()(Pred p, List<Xs...> xs) const {
            auto go = [=](auto xs) {
                return if_(p(head(xs)),
                    [=](auto xs) { return cons(head(xs), (*this)(p, tail(xs))); },
                    [](auto) { return list(); }
                )(xs);
            };
            return if_(is_empty(xs), [](auto xs) { return xs; }, go)(xs);
        }
    } take_while{};

    constexpr struct _reverse {
        template <typename ...Xs>
        constexpr auto operator()(List<Xs...> xs) const
        { return foldl(flip(cons), list(), xs); }
    } reverse{};

    constexpr struct _filter {
        template <typename Pred, typename ...Xs>
        constexpr auto operator()(Pred p, List<Xs...> xs) const {
            auto go = [=](auto x, auto xs) {
                return if_(p(x), cons(x, xs), xs);
            };
            return foldr(go, list(), xs);
        }
    } filter{};

    constexpr struct _concat {
        template <typename ...Xs>
        static constexpr List<Xs...> list_from_tuple(std::tuple<Xs...> tup)
        { return {tup}; }

        template <typename ...Lists>
        constexpr auto operator()(Lists ...lists) const
        { return list_from_tuple(std::tuple_cat(lists.storage_...)); }
    } concat{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
