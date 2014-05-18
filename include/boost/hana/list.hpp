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
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
