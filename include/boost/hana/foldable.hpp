/*!
 * @file
 * Defines `boost::hana::Foldable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Foldable;

    constexpr struct _foldl {
        template <typename F, typename State, typename Foldable_>
        constexpr auto operator()(F f, State s, Foldable_ foldable) const
        { return Foldable<Foldable_>::foldl_impl(f, s, foldable); }
    } foldl{};

    constexpr struct _foldr {
        template <typename F, typename State, typename Foldable_>
        constexpr auto operator()(F f, State s, Foldable_ foldable) const
        { return Foldable<Foldable_>::foldr_impl(f, s, foldable); }
    } foldr{};

    constexpr struct _foldr1 {
        template <typename F, typename Foldable_>
        constexpr auto operator()(F f, Foldable_ foldable) const
        { return Foldable<Foldable_>::foldr1_impl(f, foldable); }
    } foldr1{};

    constexpr struct _foldl1 {
        template <typename F, typename Foldable_>
        constexpr auto operator()(F f, Foldable_ foldable) const
        { return Foldable<Foldable_>::foldl1_impl(f, foldable); }
    } foldl1{};

    constexpr struct _minimum {
        template <typename Foldable_>
        constexpr auto operator()(Foldable_ foldable) const
        { return Foldable<Foldable_>::minimum_impl(foldable); }
    } minimum{};

    constexpr struct _maximum {
        template <typename Foldable_>
        constexpr auto operator()(Foldable_ foldable) const
        { return Foldable<Foldable_>::maximum_impl(foldable); }
    } maximum{};

    constexpr struct _maximum_by {
        template <typename Pred, typename Foldable_>
        constexpr auto operator()(Pred pred, Foldable_ foldable) const
        { return Foldable<Foldable_>::maximum_by_impl(pred, foldable); }
    } maximum_by{};

    constexpr struct _minimum_by {
        template <typename Pred, typename Foldable_>
        constexpr auto operator()(Pred pred, Foldable_ foldable) const
        { return Foldable<Foldable_>::minimum_by_impl(pred, foldable); }
    } minimum_by{};

    template <>
    struct defaults<Foldable> {
        template <typename Foldable_>
        static constexpr auto minimum_impl(Foldable_ foldable)
        { return minimum_by([](auto x, auto y) { return x < y; }, foldable); }

        template <typename Foldable_>
        static constexpr auto maximum_impl(Foldable_ foldable)
        { return maximum_by([](auto x, auto y) { return x < y; }, foldable); }

        template <typename Pred, typename Foldable_>
        static constexpr auto minimum_by_impl(Pred pred, Foldable_ foldable) {
            return foldl1(
                [=](auto x, auto y) { return if_(pred(x, y), x, y); },
                foldable
            );
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto maximum_by_impl(Pred pred, Foldable_ foldable) {
            return foldl1(
                [=](auto x, auto y) { return if_(pred(x, y), y, x); },
                foldable
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
