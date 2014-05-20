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
#include <boost/hana/detail/constexpr.hpp>
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

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = [](auto ...xs) {
        return List<decltype(xs)...>{{xs...}};
    };

    template <typename ...Xs>
    BOOST_HANA_CONSTEXPR_LAMBDA auto list_t = list(type<Xs>...);

    template <typename T, T ...xs>
    BOOST_HANA_CONSTEXPR_LAMBDA auto list_c = list(Integral<T, xs>{}...);

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
    namespace list_detail {
        template <typename ...Xs>
        static constexpr List<Xs...> from_tuple(std::tuple<Xs...> tuple)
        { return {tuple}; }

        //! @todo Remove this hack.
        template <typename T, T t>
        static constexpr SizeT<t> to_size_t(Integral<T, t>)
        { return {}; }
    }


    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto ...lists) {
        return fmap(
            [=](auto index) { return f(at(index, lists)...); },
            range(size_t<0>, minimum(list(length(lists)...)))
        );
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto ...lists) {
        return zip_with(list, lists...);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto x, auto xs) {
        return list_detail::from_tuple(std::tuple_cat(std::make_tuple(x), xs.storage_));
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto xs, auto x) {
        return list_detail::from_tuple(std::tuple_cat(xs.storage_, std::make_tuple(x)));
    };


    BOOST_HANA_CONSTEXPR_LAMBDA auto take = [](auto n, auto xs) {
        auto min = [](auto a, auto b) { return if_(a < b, a, b); };
        return fmap(
            [=](auto index) { return at(index, xs); },
            range(size_t<0>, list_detail::to_size_t(min(n, length(xs))))
        );
    };

    // Can't use a lambda because of recursion.
    //! @todo Use a Y-combinator :)
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

    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto xs) {
        return foldl(flip(cons), list(), xs);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto pred, auto xs) {
        auto go = [=](auto x, auto xs) {
            return if_(pred(x), cons, always(xs))(x, xs);
        };
        return foldr(go, list(), xs);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto concat = [](auto ...lists) {
        return list_detail::from_tuple(std::tuple_cat(lists.storage_...));
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
