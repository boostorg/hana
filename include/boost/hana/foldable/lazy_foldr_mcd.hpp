/*!
@file
Defines `boost::hana::Foldable::lazy_foldr_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_LAZY_FOLDR_MCD_HPP
#define BOOST_HANA_FOLDABLE_LAZY_FOLDR_MCD_HPP

#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/detail/maybe_fwd.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable/orderable.hpp>


namespace boost { namespace hana {
    //! @details
    //! Minimal complete definition: `lazy_foldr`
    struct Foldable::lazy_foldr_mcd {
        template <typename F, typename State, typename Foldable_>
        static constexpr auto foldr_impl(F f, State s, Foldable_ foldable)
        { return lazy_foldr(on(f, apply), s, foldable); }

        template <typename F, typename Foldable_>
        static constexpr auto foldr1_impl(F f, Foldable_ foldable) {
            auto maybe_f = [=](auto lx, auto my) {
                return just(maybe(lx(), partial(f, lx()), my()));
            };
            return from_just(lazy_foldr(maybe_f, nothing, foldable));
        }

        template <typename F, typename State, typename Foldable_>
        static constexpr auto foldl_impl(F f, State s, Foldable_ foldable) {
            auto f_ = [=](auto x, auto next) {
                return [=](auto state) {
                    return next()(f(state, x()));
                };
            };
            return lazy_foldr(f_, id, foldable)(s);
        }

        template <typename F, typename Foldable_>
        static constexpr auto foldl1_impl(F f, Foldable_ foldable) {
            auto maybe_f = [=](auto mx, auto ly) {
                return maybe(
                    just(ly()),
                    [=](auto x) { return just(f(x, ly())); },
                    mx()
                );
            };
            return from_just(lazy_foldl(maybe_f, nothing, foldable));
        }

        template <typename F, typename State, typename Foldable_>
        static constexpr auto lazy_foldl_impl(F f, State s, Foldable_ foldable) {
            auto f_ = [=](auto lx, auto lnext) {
                return [=](auto state) {
                    return lnext()(f([=] { return state; }, lx));
                };
            };
            return lazy_foldr(f_, id, foldable)(s);
        }

        template <typename Foldable_>
        static constexpr auto length_impl(Foldable_ foldable)
        { return foldl(_ + size_t<1>, size_t<0>, foldable); }

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

        template <typename Pred, typename Foldable_>
        static constexpr auto find_impl(Pred pred, Foldable_ foldable) {
            auto go = [=](auto x, auto tail) {
                return eval_if(pred(x()),
                    always(just(x())),
                    [=](auto _) { return _(tail)(); }
                );
            };
            return lazy_foldr(go, nothing, foldable);
        }

        template <typename X, typename Foldable_>
        static constexpr auto elem_impl(X x, Foldable_ foldable)
        { return any([=](auto y) { return equal(x, y); }, foldable); }

        template <typename Foldable_>
        static constexpr auto sum_impl(Foldable_ foldable)
        { return foldl(_ + _, int_<0>, foldable); }

        template <typename Foldable_>
        static constexpr auto product_impl(Foldable_ foldable)
        { return foldl(_ * _, int_<1>, foldable); }

        template <typename Pred, typename Foldable_>
        static constexpr auto count_impl(Pred pred, Foldable_ foldable) {
            auto inc = [=](auto counter, auto x) {
                return if_(pred(x), counter + size_t<1>, counter);
            };
            return foldl(inc, size_t<0>, foldable);
        }

        template <typename F, typename Foldable_>
        static constexpr auto unpack_impl(F f, Foldable_ foldable)
        { return foldl(partial, f, foldable)(); }


        // any, all, none
        template <typename Pred, typename Foldable_>
        static constexpr auto any_impl(Pred pred, Foldable_ foldable) {
            auto lazy_or = [=](auto lx, auto ly) {
                auto p = pred(lx());
                return eval_if(p, always(p), [=](auto _) { return _(ly)(); });
            };
            return lazy_foldr(lazy_or, false_, foldable);
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto all_impl(Pred pred, Foldable_ foldable)
        { return not_(any([=](auto x) { return not_(pred(x)); }, foldable)); }

        template <typename Pred, typename Foldable_>
        static constexpr auto none_impl(Pred pred, Foldable_ foldable)
        { return not_(any(pred, foldable)); }


        // any_of, all_of, none_of
        template <typename Foldable_>
        static constexpr auto any_of_impl(Foldable_ foldable)
        { return any(id, foldable); }

        template <typename Foldable_>
        static constexpr auto all_of_impl(Foldable_ foldable)
        { return all(id, foldable); }

        template <typename Foldable_>
        static constexpr auto none_of_impl(Foldable_ foldable)
        { return none(id, foldable); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_LAZY_FOLDR_MCD_HPP
