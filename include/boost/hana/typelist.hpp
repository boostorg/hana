/*!
 * @file
 * Defines `boost::hana::Typelist`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPELIST_HPP
#define BOOST_HANA_TYPELIST_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/detail/left_folds/variadic.hpp>
#include <boost/hana/detail/left_folds/variadic_meta.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    template <typename ...Xs>
    struct Typelist { };

    template <typename ...Xs>
    constexpr auto typelist = Typelist<Xs...>{};

    template <typename X, typename ...Xs>
    struct Iterable<Typelist<X, Xs...>> {
        static constexpr auto head_impl(Typelist<X, Xs...>)
        { return type<X>; }

        static constexpr auto tail_impl(Typelist<X, Xs...>)
        { return typelist<Xs...>; }

        static constexpr auto is_empty_impl(Typelist<X, Xs...>)
        { return false_; }
    };

    template <>
    struct Iterable<Typelist<>> {
        static constexpr auto is_empty_impl(Typelist<>)
        { return true_; }
    };

    template <typename ...Xs>
    struct Functor<Typelist<Xs...>> : defaults<Functor> {
        template <typename F>
        static constexpr auto fmap_impl(F f, Typelist<Xs...>)
        { return list(f(type<Xs>)...); }

        template <template <typename ...> class F>
        static constexpr auto fmap_impl(type_detail::Lift<F>, Typelist<Xs...>)
        { return typelist<F<Xs>...>; }
    };

    template <typename ...Xs>
    struct Foldable<Typelist<Xs...>> : detail::foldable_from_iterable {
        template <typename F, typename State>
        static constexpr auto foldl_impl(F f, State s, Typelist<Xs...>) {
            return detail::left_folds::variadic(f, s, type<Xs>...);
        }

        template <template <typename ...> class F, typename State>
        static constexpr auto foldl_impl(type_detail::Lift<F>, Type<State>, Typelist<Xs...>) {
            return type<detail::left_folds::variadic_meta<F, State, Xs...>>;
        }
    };

    template <typename ...Xs, typename ...Ys>
    struct Comparable<Typelist<Xs...>, Typelist<Ys...>>
        : detail::comparable_from_iterable
    { };

    template <typename ...Xs, typename ...Ys>
    constexpr auto operator==(Typelist<Xs...> xs, Typelist<Ys...> ys)
    {  return equal(xs, ys); }

    template <typename ...Xs, typename ...Ys>
    constexpr auto operator!=(Typelist<Xs...> xs, Typelist<Ys...> ys)
    {  return not_equal(xs, ys); }
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPELIST_HPP
