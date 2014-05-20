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
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Foldable;

    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl = [](auto f, auto state, auto foldable) {
        return Foldable<decltype(foldable)>::foldl_impl(f, state, foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr = [](auto f, auto state, auto foldable) {
        return Foldable<decltype(foldable)>::foldr_impl(f, state, foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr1 = [](auto f, auto foldable) {
        return Foldable<decltype(foldable)>::foldr1_impl(f, foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl1 = [](auto f, auto foldable) {
        return Foldable<decltype(foldable)>::foldl1_impl(f, foldable);
    };


    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum_by = [](auto pred, auto foldable) {
        return Foldable<decltype(foldable)>::minimum_by_impl(pred, foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum = [](auto foldable) {
        return Foldable<decltype(foldable)>::minimum_impl(foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum_by = [](auto pred, auto foldable) {
        return Foldable<decltype(foldable)>::maximum_by_impl(pred, foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum = [](auto foldable) {
        return Foldable<decltype(foldable)>::maximum_impl(foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto foldable) {
        return Foldable<decltype(foldable)>::sum_impl(foldable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto product = [](auto foldable) {
        return Foldable<decltype(foldable)>::product_impl(foldable);
    };


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

        template <typename Foldable_>
        static constexpr auto sum_impl(Foldable_ foldable)
        { return foldl([](auto x, auto y) { return x + y; }, int_<0>, foldable); }

        template <typename Foldable_>
        static constexpr auto product_impl(Foldable_ foldable)
        { return foldl([](auto x, auto y) { return x * y; }, int_<1>, foldable); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
