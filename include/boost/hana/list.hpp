/*!
@file
Defines `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_HPP
#define BOOST_HANA_LIST_HPP

#include <boost/hana/fwd/list.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/product.hpp>

// provided instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    template <typename L>
    struct make_impl<L, when<is_a<List, L>{}>> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return detail::variadic::foldr(cons, nil<L>(),
                detail::std::forward<Xs>(xs)...
            );
        }
    };

    template <typename Xs, typename>
    struct concat_impl : concat_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct concat_impl<L, when<condition>> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return foldr(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys),
                cons
            );
        }
    };

    template <typename Xs, typename>
    struct filter_impl : filter_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct filter_impl<L, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            auto go = [=](auto x, auto xs) {
                return eval_if(pred(x),
                    [=](auto _) { return _(cons)(x, xs); },
                    always(xs)
                );
            };
            return foldr(xs, nil<L>(), go);
        }
    };

    template <typename Xs, typename>
    struct group_by_impl : group_by_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct group_by_impl<L, when<condition>> {
        template <typename Pred, typename Xs>
        static constexpr auto apply(Pred pred, Xs xs_) {
            return eval_if(is_empty(xs_),
                [](auto) { return nil<L>(); },
                [=](auto _) {
                    auto x = _(head)(xs_);
                    auto xs = _(tail)(xs_);
                    auto ys_zs = span(xs, [=](auto y) { return pred(x, y); });
                    auto ys = first(ys_zs);
                    auto zs = second(ys_zs);
                    return cons(cons(x, ys), apply(pred, zs));
                }
            );
        }
    };

    template <typename Xs, typename>
    struct group_impl : group_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct group_impl<L, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return group_by(equal, detail::std::forward<Xs>(xs));
        }
    };

    template <typename Xs, typename>
    struct init_impl : init_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct init_impl<L, when<condition>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return eval_if(is_empty(tail(xs)),
                always(nil<L>()),
                [=](auto _) { return cons(_(head)(xs), apply(_(tail)(xs))); }
            );
        }
    };

    template <typename Xs, typename>
    struct intersperse_impl : intersperse_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct intersperse_impl<L, when<condition>> {
        template <typename Prefix, typename Xs>
        static constexpr decltype(auto) prepend_to_all(Prefix&& prefix, Xs&& xs) {
            return foldr(detail::std::forward<Xs>(xs), nil<L>(),
                [pfx(detail::std::forward<Prefix>(prefix))](auto&& x, auto&& xs) -> decltype(auto) {
                    return cons(pfx,
                        cons(
                            detail::std::forward<decltype(x)>(x),
                            detail::std::forward<decltype(xs)>(xs)
                        )
                    );
                }
            );
        }

        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            return eval_if(is_empty(xs),
                [&xs](auto _) -> decltype(auto) {
                    return id(detail::std::forward<Xs>(xs));
                },
                [&xs, &z](auto _) -> decltype(auto) {
                    return cons(
                        _(head)(xs),
                        prepend_to_all(
                            detail::std::forward<Z>(z),
                            _(tail)(xs)
                        )
                    );
                }
            );
        }
    };

    template <typename Xs, typename>
    struct partition_impl : partition_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct partition_impl<L, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            auto go = [=](auto parts, auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return pair(_(snoc)(first(parts), x), second(parts)); },
                    [=](auto _) { return pair(first(parts), _(snoc)(second(parts), x)); }
                );
            };
            return foldl(xs, pair(nil<L>(), nil<L>()), go);
        }
    };

    template <typename Xs, typename>
    struct permutations_impl : permutations_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct permutations_impl<L, when<condition>> {
        template <typename X, typename Xs>
        static constexpr auto insertions(X x, Xs l) {
            return eval_if(is_empty(l),
                [=](auto _) { return lift<L>(lift<L>(x)); },
                [=](auto _) {
                    auto y = _(head)(l);
                    auto ys = _(tail)(l);
                    return cons(
                        _(cons)(x, l),
                        transform(insertions(x, ys), [=](auto zs) {
                            return cons(y, zs);
                        })
                    );
                }
            );
        }

        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return eval_if(is_empty(xs),
                [](auto _) { return lift<L>(nil<L>()); },
                [=](auto _) {
                    return flatten(transform(apply(_(tail)(xs)), [=](auto ys) {
                        return insertions(_(head)(xs), ys);
                    }));
                }
            );
        }
    };

    template <typename Xs, typename>
    struct remove_at_impl : remove_at_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct remove_at_impl<L, when<condition>> {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N&& n, Xs&& xs) {
            using I = typename datatype<N>::type;
            return eval_if(equal(n, zero<I>()),
                [&xs](auto _) -> decltype(auto) {
                    return _(tail)(detail::std::forward<Xs>(xs));
                },
                [&xs, &n](auto _) -> decltype(auto) {
                    return cons(
                        _(head)(xs),
                        apply(_(pred)(n), _(tail)(xs))
                    );
                }
            );
        }
    };

    //! @todo
    //! Here, we syntactically only need `make`, but we _actually_ need
    //! `make` to behave properly. With this definition, it is possible
    //! to call `repeat<Pair>(...)`, which is wrong.
    template <typename Xs, typename>
    struct repeat_impl : repeat_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct repeat_impl<L, when<condition>> {
        template <typename X, int ...i>
        static constexpr decltype(auto)
        repeat_helper(X&& x, detail::std::integer_sequence<int, i...>)
        { return make<L>(((void)i, x)...); }

        template <typename N, typename X>
        static constexpr auto apply(N n, X x) {
            constexpr auto m = value(n);
            return repeat_helper(x, detail::std::make_integer_sequence<int, m>{});
        }
    };

    template <typename Xs, typename>
    struct reverse_impl : reverse_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct reverse_impl<L, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return foldl(detail::std::forward<Xs>(xs), nil<L>(), flip(cons));
        }
    };

    template <typename Xs, typename>
    struct scanl_impl : scanl_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct scanl_impl<L, when<condition>> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) {
            return eval_if(is_empty(xs),
                [=](auto _) { return lift<L>(s); },
                [=](auto _) {
                    return cons(s, apply(_(tail)(xs), f(s, _(head)(xs)), f));
                }
            );
        }
    };

    template <typename Xs, typename>
    struct scanl1_impl : scanl1_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct scanl1_impl<L, when<condition>> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return eval_if(is_empty(xs),
                always(nil<L>()),
                [=](auto _) { return scanl(_(tail)(xs), _(head)(xs), f); }
            );
        }
    };

    template <typename Xs, typename>
    struct scanr_impl : scanr_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct scanr_impl<L, when<condition>> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) {
            return eval_if(is_empty(xs),
                [=](auto _) { return lift<L>(s); },
                [=](auto _) {
                    auto rest = apply(_(tail)(xs), s, f);
                    return cons(f(_(head)(xs), head(rest)), rest);
                }
            );
        }
    };

    template <typename Xs, typename>
    struct scanr1_impl : scanr1_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct scanr1_impl<L, when<condition>> {
        template <typename Lst, typename F>
        static constexpr auto apply(Lst lst, F f) {
            return eval_if(is_empty(lst),
                always(nil<L>()),
                [=](auto _) {
                    auto x = _(head)(lst);
                    auto xs = _(tail)(lst);
                    return eval_if(is_empty(xs),
                        [=](auto _) { return lift<L>(x); },
                        [=](auto _) {
                            auto rest = apply(xs, _(f));
                            return cons(f(x, head(rest)), rest);
                        }
                    );
                }
            );
        }
    };

    template <typename Xs, typename>
    struct slice_impl : slice_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct slice_impl<L, when<condition>> {
        template <typename Xs, typename From, typename To>
        static constexpr auto apply(Xs&& xs, From const& from, To const& to) {
            return take(minus(to, from), drop(from, detail::std::forward<Xs>(xs)));
        }
    };

    template <typename Xs, typename>
    struct snoc_impl : snoc_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct snoc_impl<L, when<condition>> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return foldr(
                detail::std::forward<Xs>(xs),
                lift<L>(detail::std::forward<X>(x)),
                cons
            );
        }
    };

    template <typename Xs, typename>
    struct sort_impl : sort_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct sort_impl<L, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return sort_by(less, detail::std::forward<Xs>(xs)); }
    };

    template <typename Xs, typename>
    struct sort_by_impl : sort_by_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct sort_by_impl<L, when<condition>> {
        template <typename Pred, typename Xs>
        static constexpr auto apply(Pred pred, Xs xs) {
            return eval_if(is_empty(xs),
                always(xs),
                [=](auto _) {
                    return eval_if(is_empty(_(tail)(xs)),
                        always(xs),
                        [=](auto _) {
                            auto pivot = _(head)(xs);
                            auto rest = _(tail)(xs);
                            auto parts = partition(rest, [=](auto x) { return pred(x, pivot); });
                            return concat(
                                apply(pred, first(parts)),
                                cons(pivot, apply(pred, second(parts)))
                            );
                        }
                    );
                }
            );
        }
    };

    template <typename Xs, typename>
    struct span_impl : span_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct span_impl<L, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                [=](auto _) { return pair(nil<L>(), nil<L>()); },
                [=](auto _) {
                    auto x = _(head)(xs);
                    auto xs_ = _(tail)(xs);
                    return eval_if(pred(x),
                        [=](auto _) {
                            auto ys_zs = apply(xs_, _(pred));
                            auto ys = first(ys_zs);
                            auto zs = second(ys_zs);
                            return pair(cons(x, ys), zs);
                        },
                        [=](auto _) {
                            return _(pair)(nil<L>(), xs);
                        }
                    );
                }
            );
        }
    };

    template <typename Xs, typename>
    struct take_impl : take_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct take_impl<L, when<condition>> {
        template <typename N, typename Xs>
        static constexpr auto apply(N n, Xs xs) {
            return eval_if(or_(is_empty(xs), equal(n, int_<0>)),
                always(nil<L>()),
                [=](auto _) {
                    return cons(_(head)(xs), apply(minus(n, int_<1>), _(tail)(xs)));
                }
            );
        }
    };

    template <typename Xs, typename>
    struct take_until_impl : take_until_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct take_until_impl<L, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return take_while(
                detail::std::forward<Xs>(xs),
                compose(not_, detail::std::forward<Pred>(pred))
            );
        }
    };

    template <typename Xs, typename>
    struct take_while_impl : take_while_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct take_while_impl<L, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                always(nil<L>()),
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) {
                            return cons(
                                _(head)(xs),
                                apply(_(tail)(xs), pred)
                            );
                        },
                        always(nil<L>())
                    );
                }
            );
        }
    };

    template <typename Xs, typename>
    struct unfoldr_impl : unfoldr_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct unfoldr_impl<L, when<condition>> {
        template <typename F, typename Init>
        static constexpr auto apply(F f, Init init) {
            auto g = [=](auto a_b) {
                return cons(first(a_b), apply(f, second(a_b)));
            };
            return maybe(nil<L>(), g, f(init));
        }
    };

    template <typename Xs, typename>
    struct unfoldl_impl : unfoldl_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct unfoldl_impl<L, when<condition>> {
        template <typename F, typename Init>
        static constexpr auto apply(F f, Init init) {
            auto g = [=](auto b_a) {
                return snoc(apply(f, first(b_a)), second(b_a));
            };
            return maybe(nil<L>(), g, f(init));
        }
    };

    template <typename Xs, typename>
    struct unzip_impl : unzip_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct unzip_impl<L, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return unpack(detail::std::forward<Xs>(xs), zip); }
    };

    template <typename Xs, typename>
    struct zip_impl : zip_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct zip_impl<L, when<condition>> {
        template <typename ...Xss>
        static constexpr decltype(auto) apply(Xss&& ...xss)
        { return zip_with(make<L>, detail::std::forward<Xss>(xss)...); }
    };

    template <typename Xs, typename>
    struct zip_with_impl : zip_with_impl<Xs, when<true>> { };

    template <typename L, bool condition>
    struct zip_with_impl<L, when<condition>> {
        template <typename F, typename ...Xss>
        static constexpr auto apply(F f, Xss ...xss) {
            return eval_if(or_(is_empty(xss)...),
                [](auto _) { return nil<L>(); },
                [=](auto _) {
                    return cons(
                        f(_(head)(xss)...),
                        apply(f, _(tail)(xss)...)
                    );
                }
            );
        }
    };

    //! Converts a `Foldable` to a `List`.
    template <typename L, typename T>
    struct convert<L, T, when<is_a<List, L>{} && is_a<Foldable, T>{}>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return foldr(detail::std::forward<Xs>(xs), nil<L>(), cons); }
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
    struct lift_impl<T, when<is_a<List, T>{}>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return cons(detail::std::forward<X>(x), nil<T>()); }
    };

    template <typename T>
    struct ap_impl<T, when<is_a<List, T>{}>> {
        template <typename Fs, typename Xs>
        static constexpr decltype(auto) apply(Fs&& fs, Xs&& xs) {
            return bind(
                detail::std::forward<Fs>(fs),
                partial(transform, detail::std::forward<Xs>(xs))
            );
        }
    };

    //! Instance of `Comparable` for instances of `List`.
    //!
    //! Two `List`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //!
    //! ### Example
    //! @snippet example/list/comparable.cpp main
    template <typename T, typename U>
    struct equal_impl<T, U, when<is_a<List, T>{} && is_a<List, U>{}>> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs const& xs, Ys const& ys) {
            return eval_if(or_(is_empty(xs), is_empty(ys)),
                [&xs, &ys](auto _) -> decltype(auto) {
                    return and_(_(is_empty)(xs), _(is_empty)(ys));
                },
                [&xs, &ys](auto _) -> decltype(auto) {
                    return and_(equal(_(head)(xs), _(head)(ys)),
                                apply(_(tail)(xs), _(tail)(ys)));
                }
            );
        }
    };

    //! `Functor` instance for `List`s.
    //!
    //! `List`s implement `transform` as the mapping of a function over each
    //! element of the list, which is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //!
    //! ### Example
    //! @snippet example/list/functor.cpp transform
    template <typename T>
    struct transform_impl<T, when<is_a<List, T>{}>> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return foldr(
                detail::std::forward<Xs>(xs),
                nil<T>(),
                compose(cons, detail::std::forward<F>(f))
            );
        }
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
    struct flatten_impl<T, when<is_a<List, T>{}>> {
        template <typename Xss>
        static constexpr decltype(auto) apply(Xss&& xss) {
            return foldl(detail::std::forward<Xss>(xss), nil<T>(), concat);
        }
    };

    //! `Traversable` instance for `List` instances.
    //!
    //! ### Example
    //! @snippet example/list/traversable.cpp main
    template <typename T>
    struct traverse_impl<T, when<is_a<List, T>{}>> {
        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return foldr(detail::std::forward<Xs>(xs), lift<A>(nil<T>()),
            [f(detail::std::forward<F>(f))](auto&& x, auto&& ys) -> decltype(auto) {
                return ap(
                    transform(
                        f(detail::std::forward<decltype(x)>(x)),
                        curry<2>(cons)
                    ),
                    detail::std::forward<decltype(ys)>(ys)
                );
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
