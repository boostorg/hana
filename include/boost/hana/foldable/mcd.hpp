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
        template <typename Foldable_, typename F>
        static constexpr auto foldr1_impl(Foldable_ foldable, F f) {
            auto g = [=](auto x, auto mstate) {
                return just(maybe(
                    x,
                    [=](auto state) { return f(x, state); },
                    mstate
                ));
            };
            return from_just(foldr(foldable, nothing, g));
        }

        template <typename Foldable_, typename F>
        static constexpr auto foldl1_impl(Foldable_ foldable, F f) {
            auto g = [=](auto mstate, auto x) {
                return maybe(
                    just(x),
                    [=](auto state) { return just(f(state, x)); },
                    mstate
                );
            };
            return from_just(foldl(foldable, nothing, g));
        }

        template <typename Foldable_>
        static constexpr auto length_impl(Foldable_ foldable) {
            auto plus1 = [](auto n, auto _) { return n + size_t<1>; };
            return foldl(foldable, size_t<0>, plus1);
        }

        template <typename Foldable_>
        static constexpr auto minimum_impl(Foldable_ foldable)
        { return minimum_by(less, foldable); }

        template <typename Foldable_>
        static constexpr auto maximum_impl(Foldable_ foldable)
        { return maximum_by(less, foldable); }

        template <typename Pred, typename Foldable_>
        static constexpr auto minimum_by_impl(Pred pred, Foldable_ foldable) {
            return foldl1(foldable, [=](auto x, auto y) {
                return if_(pred(x, y), x, y);
            });
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto maximum_by_impl(Pred pred, Foldable_ foldable) {
            return foldl1(foldable, [=](auto x, auto y) {
                return if_(pred(x, y), y, x);
            });
        }

        template <typename Foldable_>
        static constexpr auto sum_impl(Foldable_ foldable) {
            return foldl(foldable, int_<0>, [](auto x, auto y) {
                return x + y;
            });
        }

        template <typename Foldable_>
        static constexpr auto product_impl(Foldable_ foldable) {
            return foldl(foldable, int_<1>, [](auto x, auto y) {
                return x * y;
            });
        }

        template <typename Foldable_, typename Pred>
        static constexpr auto count_impl(Foldable_ foldable, Pred pred) {
            return foldl(foldable, size_t<0>, [=](auto counter, auto x) {
                return if_(pred(x), counter + size_t<1>, counter);
            });
        }

        template <typename Foldable_, typename F>
        static constexpr auto unpack_impl(Foldable_ foldable, F f) {
            return foldl(foldable, f, [](auto g, auto x) {
                return [=](auto ...y) { return g(x, y...); };
            })();
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_MCD_HPP
