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

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/detail/right_folds/variadic_unrolled.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>
#include <boost/hana/orderable/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/searchable/mcd.hpp>
#include <boost/hana/traversable/traverse_mcd.hpp>


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

        template <typename F, typename Init>
        static constexpr auto unfoldr_impl(F f, Init init) {
            auto g = [=](auto a_b) {
                return cons(first(a_b), unfoldr_impl(f, second(a_b)));
            };
            return maybe(nil<T>, g, f(init));
        }

        template <typename F, typename Init>
        static constexpr auto unfoldl_impl(F f, Init init) {
            auto g = [=](auto b_a) {
                return snoc(unfoldl_impl(f, first(b_a)), second(b_a));
            };
            return maybe(nil<T>, g, f(init));
        }

        template <typename Xs>
        static constexpr auto unzip_impl(Xs xs)
        { return unpack(zip, xs); }

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

    //! @details
    //! `List`s implement `fmap` as the mapping of a function over each
    //! element of the list. This is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //!
    //! ### Example 1
    //! @snippet example/list/functor/fmap.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/functor/fmap.cpp main
    template <typename T>
    struct Functor::instance<T, when<is_a<List, T>()>>
        : Functor::fmap_mcd
    {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            auto cons_f = [=](auto x, auto xs) { return cons(f(x), xs); };
            return foldr(cons_f, nil<T>, xs);
        }
    };

    //! `Applicative` instance for instances of the `List` type class.
    //!
    //! A value can be lifted into a singleton list with `lift`. `ap(fs, xs)`
    //! applies each function in the list `fs` to each value in the list `xs`,
    //! and returns a list containing all the results.
    //!
    //! ### Example
    //! @snippet example/list/applicative.cpp main
    template <typename T>
    struct Applicative::instance<T, when<is_a<List, T>()>>
        : Applicative::mcd
    {
        template <typename X>
        static constexpr auto lift_impl(X x)
        { return cons(x, nil<T>); }

        template <typename Fs, typename Xs>
        static constexpr auto ap_impl(Fs fs, Xs xs)
        { return bind(fs, [=](auto f) { return fmap(f, xs); }); }
    };

    //! `Monad` instance for instances of the `List` type class.
    //!
    //! A function returning a list of results can be mapped over all the
    //! elements of a list and have all the results concatenated using `bind`.
    //! Also, a list of lists can be flattened one level with `flatten`.
    //!
    //! ### Example
    //! @snippet example/list/monad.cpp main
    template <typename T>
    struct Monad::instance<T, when<is_a<List, T>()>>
        : Monad::flatten_mcd<T>
    {
        template <typename Xss>
        static constexpr auto flatten_impl(Xss xss)
        { return foldl(concat, nil<T>, xss); }
    };

    //! `Traversable` instance for `List` instances.
    //!
    //! ### Example
    //! @snippet example/list/traversable/traverse.cpp main
    //!
    //! ### Example
    //! @snippet example/list/traversable/sequence.cpp main
    template <typename T>
    struct Traversable::instance<T, when<is_a<List, T>()>>
        : Traversable::traverse_mcd
    {
        template <typename A, typename F, typename Xs>
        static constexpr auto traverse_impl(F f, Xs xs) {
            auto curried_cons = [](auto x) {
                return [=](auto xs) { return cons(x, xs); };
            };
            auto cons_f = [=](auto x, auto ys) {
                return ap(fmap(curried_cons, f(x)), ys);
            };
            return foldr(cons_f, lift<A>(nil<T>), xs);
        }
    };

    //! @details
    //! Two `List`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //!
    //! ### Example
    //! @snippet example/list/comparable.cpp main
    template <typename T, typename U>
    struct Comparable::instance<T, U, when<
        is_a<List, T>() && is_a<List, U>()
    >> : Comparable::equal_mcd
    {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys) {
            return eval_if(or_(is_empty(xs), is_empty(ys)),
                [=](auto _) {
                    return and_(_(is_empty)(xs), _(is_empty)(ys));
                },
                [=](auto _) {
                    return and_(equal(_(head)(xs), _(head)(ys)),
                                equal_impl(_(tail)(xs), _(tail)(ys)));
                }
            );
        }
    };

    //! @details
    //! A `List` can be searched by doing a linear search in the elements,
    //! with the keys and values being both the elements in the list.
    //!
    //! @todo
    //! Technically, this can be implemented in `Iterable`. Should it?
    //!
    //! ### Example
    //! @snippet example/list/searchable/find.cpp main
    template <typename T>
    struct Searchable::instance<T, when<is_a<List, T>()>>
        : Searchable::mcd
    {
        template <typename Pred, typename Xs>
        static constexpr auto find_impl(Pred pred, Xs xs) {
            auto e = drop_until(pred, xs);
            return eval_if(is_empty(e),
                [](auto) { return nothing; },
                [=](auto _) { return just(_(head)(e)); }
            );
        }

        template <typename Pred, typename Xs>
        static constexpr auto any_impl(Pred pred, Xs xs) {
            return eval_if(is_empty(xs),
                [](auto _) { return false_; },
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) { return true_; },
                        [=](auto _) { return any_impl(pred, _(tail)(xs)); }
                    );
                }
            );
        }
    };

    //! Converts a `Foldable` to a `List`.
    template <typename L, typename T>
    struct convert<L, T, detail::std::enable_if_t<
        is_a<List, L>() && is_a<Foldable, T>()
    >> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return foldr(cons, nil<L>, xs); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_MCD_HPP
