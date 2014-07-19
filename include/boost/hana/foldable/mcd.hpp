/*!
@file
Defines `boost::hana::Foldable::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_MCD_HPP
#define BOOST_HANA_FOLDABLE_MCD_HPP

#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/integral.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/orderable/orderable.hpp>


namespace boost { namespace hana {
    //! @details
    //! Minimal complete definition: `foldl` and `foldr`
    struct Foldable::mcd {
        template <typename F, typename Foldable_>
        static constexpr auto foldr1_impl(F f, Foldable_ foldable) {
            auto g = [=](auto x, auto mstate) {
                return just(maybe(
                    x,
                    [=](auto state) { return f(x, state); },
                    mstate
                ));
            };
            return from_just(foldr(g, nothing, foldable));
        }

        template <typename F, typename Foldable_>
        static constexpr auto foldl1_impl(F f, Foldable_ foldable) {
            auto g = [=](auto mstate, auto x) {
                return maybe(
                    just(x),
                    [=](auto state) { return just(f(state, x)); },
                    mstate
                );
            };
            return from_just(foldl(g, nothing, foldable));
        }

        template <typename Foldable_>
        static constexpr auto length_impl(Foldable_ foldable) {
            auto plus1 = [](auto n, auto _) { return n + size_t<1>; };
            return foldl(plus1, size_t<0>, foldable);
        }

        template <typename Foldable_>
        static constexpr auto minimum_impl(Foldable_ foldable)
        { return minimum_by(less, foldable); }

        template <typename Foldable_>
        static constexpr auto maximum_impl(Foldable_ foldable)
        { return maximum_by(less, foldable); }

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
        static constexpr auto sum_impl(Foldable_ foldable) {
            auto add = [](auto x, auto y) { return x + y; };
            return foldl(add, int_<0>, foldable);
        }

        template <typename Foldable_>
        static constexpr auto product_impl(Foldable_ foldable) {
            auto mul = [](auto x, auto y) { return x * y; };
            return foldl(mul, int_<1>, foldable);
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto count_impl(Pred pred, Foldable_ foldable) {
            auto inc = [=](auto counter, auto x) {
                return if_(pred(x), counter + size_t<1>, counter);
            };
            return foldl(inc, size_t<0>, foldable);
        }

        template <typename F, typename Foldable_>
        static constexpr auto unpack_impl(F f, Foldable_ foldable) {
            auto partial = [](auto f, auto ...x) {
                return [=](auto ...y) { return f(x..., y...); };
            };
            return foldl(partial, f, foldable)();
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_MCD_HPP
