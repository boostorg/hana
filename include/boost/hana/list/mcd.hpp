/*!
@file
Defines `boost::hana::List::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_MCD_HPP
#define BOOST_HANA_LIST_MCD_HPP

#include <boost/hana/list/list.hpp>

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/detail/right_folds/variadic_unrolled.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monad/monad.hpp>
#include <boost/hana/orderable/orderable.hpp>
#include <boost/hana/pair/instance.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `Iterable`, `Foldable`, `cons`, and `nil`
    template <typename T>
    struct List::mcd {
        template <typename Xs, typename Ys>
        static constexpr auto concat_impl(Xs xs, Ys ys)
        { return foldr(cons, ys, xs); }

        template <typename Pred, typename Xs>
        static constexpr auto filter_impl(Pred pred, Xs xs) {
            auto go = [=](auto x, auto xs) {
                return eval_if(pred(x),
                    [=](auto _) { return _(cons)(x, xs); },
                    [=](auto _) { return xs; }
                );
            };
            return foldr(go, nil<T>, xs);
        }

        template <typename Xs>
        static constexpr auto init_impl(Xs xs) {
            return eval_if(is_empty(tail(xs)),
                [](auto _) { return nil<T>; },
                [=](auto _) { return cons(_(head)(xs), init_impl(_(tail)(xs))); }
            );
        }

        template <typename ...Xs>
        static constexpr auto into_impl(Xs ...xs) {
            return detail::right_folds::variadic_unrolled(cons, nil<T>, xs...);
        }

        template <typename Pred, typename Xs>
        static constexpr auto partition_impl(Pred pred, Xs xs) {
            auto go = [=](auto parts, auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return pair(_(snoc)(first(parts), x), second(parts)); },
                    [=](auto _) { return pair(first(parts), _(snoc)(second(parts), x)); }
                );
            };
            return foldl(go, pair(nil<T>, nil<T>), xs);
        }

    private:
        template <typename X, typename Xs>
        static constexpr auto insertions(X x, Xs l) {
            return eval_if(is_empty(l),
                [=](auto _) { return lift<T>(lift<T>(x)); },
                [=](auto _) {
                    auto y = _(head)(l);
                    auto ys = _(tail)(l);
                    return cons(
                        _(cons)(x, l),
                        fmap([=](auto zs) { return cons(y, zs); }, insertions(x, ys))
                    );
                }
            );
        }

    public:
        template <typename Xs>
        static constexpr auto permutations_impl(Xs xs) {
            return eval_if(is_empty(xs),
                [](auto _) { return lift<T>(nil<T>); },
                [=](auto _) {
                    return flatten(fmap(
                        [=](auto ys) { return insertions(_(head)(xs), ys); },
                        permutations_impl(_(tail)(xs))
                    ));
                }
            );
        }

        template <typename Xs>
        static constexpr auto reverse_impl(Xs xs) {
            auto flip_cons = [](auto xs, auto x) { return cons(x, xs); };
            return foldl(flip_cons, nil<T>, xs);
        }

        template <typename F, typename S, typename Xs>
        static constexpr auto scanl_impl(F f, S s, Xs xs) {
            return eval_if(is_empty(xs),
                [=](auto _) { return lift<T>(s); },
                [=](auto _) {
                    return cons(s, scanl_impl(f, f(s, _(head)(xs)), _(tail)(xs)));
                }
            );
        }

        template <typename F, typename Xs>
        static constexpr auto scanl1_impl(F f, Xs xs) {
            return eval_if(is_empty(xs),
                [](auto _) { return nil<T>; },
                [=](auto _) { return scanl(f, _(head)(xs), _(tail)(xs)); }
            );
        }

        template <typename F, typename S, typename Xs>
        static constexpr auto scanr_impl(F f, S s, Xs xs) {
            return eval_if(is_empty(xs),
                [=](auto _) { return lift<T>(s); },
                [=](auto _) {
                    auto rest = scanr_impl(f, s, _(tail)(xs));
                    return cons(f(_(head)(xs), head(rest)), rest);
                }
            );
        }

        template <typename F, typename Lst>
        static constexpr auto scanr1_impl(F f, Lst lst) {
            return eval_if(is_empty(lst),
                [](auto _) { return nil<T>; },
                [=](auto _) {
                    auto x = _(head)(lst);
                    auto xs = _(tail)(lst);
                    return eval_if(is_empty(xs),
                        [=](auto _) { return lift<T>(x); },
                        [=](auto _) {
                            auto rest = scanr1_impl(_(f), xs);
                            return cons(f(x, head(rest)), rest);
                        }
                    );
                }
            );
        }

        template <typename Xs, typename X>
        static constexpr auto snoc_impl(Xs xs, X x) {
            return foldr(cons, lift<T>(x), xs);
        }

        template <typename Xs>
        static constexpr auto sort_impl(Xs xs) {
            return sort_by(less, xs);
        }

        template <typename Pred, typename Xs>
        static constexpr auto sort_by_impl(Pred pred, Xs xs) {
            return eval_if(is_empty(xs),
                [=](auto _) { return xs; },
                [=](auto _) {
                    return eval_if(is_empty(_(tail)(xs)),
                        [=](auto _) { return xs; },
                        [=](auto _) {
                            auto pivot = _(head)(xs);
                            auto rest = _(tail)(xs);
                            auto parts = partition([=](auto x) { return pred(x, pivot); }, rest);
                            return concat(
                                sort_by_impl(pred, first(parts)),
                                cons(pivot, sort_by_impl(pred, second(parts)))
                            );
                        }
                    );
                }
            );
        }

        template <typename N, typename Xs>
        static constexpr auto take_impl(N n, Xs xs) {
            return eval_if(or_(is_empty(xs), equal(n, int_<0>)),
                [](auto _) { return nil<T>; },
                [=](auto _) {
                    return cons(_(head)(xs), take_impl(n - int_<1>, _(tail)(xs)));
                }
            );
        }

        template <typename Pred, typename Xs>
        static constexpr auto take_until_impl(Pred pred, Xs xs) {
            return take_while([=](auto x) { return not_(pred(x)); }, xs);
        }

        template <typename Pred, typename Xs>
        static constexpr auto take_while_impl(Pred pred, Xs xs) {
            return eval_if(is_empty(xs),
                [=](auto _) { return nil<T>; },
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) {
                            return cons(
                                _(head)(xs),
                                take_while_impl(pred, _(tail)(xs))
                            );
                        },
                        [=](auto _) { return nil<T>; }
                    );
                }
            );
        }

        template <typename ...Xss>
        static constexpr auto zip_impl(Xss ...xss)
        { return zip_with(into<T>, xss...); }

        template <typename F, typename ...Xss>
        static constexpr auto zip_with_impl(F f, Xss ...xss) {
            return eval_if(any_of(into<T>(is_empty(xss)...)),
                [](auto _) { return nil<T>; },
                [=](auto _) {
                    return cons(
                        f(_(head)(xss)...),
                        zip_with_impl(f, _(tail)(xss)...)
                    );
                }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_MCD_HPP
