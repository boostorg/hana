/*!
@file
Defines `boost::hana::Sequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEQUENCE_HPP
#define BOOST_HANA_SEQUENCE_HPP

#include <boost/hana/fwd/sequence.hpp>

namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // group_by
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct group_by_impl : group_by_impl<S, when<true>> { };

    namespace sequence_detail {
        struct group_by_loop {
            template <typename Pred, typename Grouped, typename Xs>
            constexpr decltype(auto) operator()(Pred&& pred, Grouped&& grouped, Xs&& xs) const {
                decltype(auto) x = hana::head(xs);
                decltype(auto) tmp = hana::span(detail::std::forward<Xs>(xs),
                    hana::partial(detail::std::forward<Pred>(pred),
                                  detail::std::forward<decltype(x)>(x)));
                decltype(auto) spn = hana::first(detail::std::forward<decltype(tmp)>(tmp));
                decltype(auto) rest = hana::second(detail::std::forward<decltype(tmp)>(tmp));
                return hana::pair(
                    hana::append(detail::std::forward<Grouped>(grouped),
                                 detail::std::forward<decltype(spn)>(spn)),
                    detail::std::forward<decltype(rest)>(rest)
                );
            }
        };
    }

    template <typename S, bool condition>
    struct group_by_impl<S, when<condition>> {
        template <typename Pred, typename Xs>
        static constexpr decltype(auto) apply(Pred&& pred, Xs&& xs) {
            return hana::first(hana::until(hana::compose(is_empty, second),
                hana::pair(nil<S>(), detail::std::forward<Xs>(xs)),
                hana::fuse(hana::partial(sequence_detail::group_by_loop{},
                                         detail::std::forward<Pred>(pred)))
            ));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // group
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct group_impl : group_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct group_impl<S, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::group_by(equal, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // init
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct init_impl : init_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct init_impl<S, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs xs) {
            return eval_if(is_empty(tail(xs)),
                always(nil<S>()),
                [=](auto _) { return hana::prepend(_(head)(xs), apply(_(tail)(xs))); }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // intersperse
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct intersperse_impl : intersperse_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct intersperse_impl<S, when<condition>> {
        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            return hana::eval_if(hana::is_empty(xs),
                [&xs](auto _) -> decltype(auto) {
                    return hana::id(detail::std::forward<Xs>(xs));
                },
                [&xs, &z](auto _) -> decltype(auto) {
                    return hana::cons(
                        _(head)(xs),
                        hana::prefix(detail::std::forward<Z>(z), _(tail)(xs))
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // partition
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct partition_impl : partition_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct partition_impl<S, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            auto go = [=](auto parts, auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return pair(_(append)(first(parts), x), second(parts)); },
                    [=](auto _) { return pair(first(parts), _(append)(second(parts), x)); }
                );
            };
            return foldl(xs, pair(nil<L>(), nil<L>()), go);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // permutations
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct permutations_impl : permutations_impl<Xs, when<true>> { };

    template <typename S, bool condition>
    struct permutations_impl<S, when<condition>> {
        template <typename X, typename Xs>
        static constexpr auto insertions(X x, Xs l) {
            return eval_if(is_empty(l),
                [=](auto _) { return lift<S>(lift<S>(x)); },
                [=](auto _) {
                    auto y = _(head)(l);
                    auto ys = _(tail)(l);
                    return prepend(
                        _(prepend)(x, l),
                        transform(insertions(x, ys), partial(prepend, y))
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

    //////////////////////////////////////////////////////////////////////////
    // remove_at
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct remove_at_impl : remove_at_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct remove_at_impl<S, when<condition>> {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N&& n, Xs&& xs) {
            using I = typename datatype<N>::type;
            return eval_if(equal(n, zero<I>()),
                [&xs](auto _) -> decltype(auto) {
                    return _(tail)(detail::std::forward<Xs>(xs));
                },
                [&xs, &n](auto _) -> decltype(auto) {
                    return prepend(
                        _(head)(xs),
                        apply(_(pred)(n), _(tail)(xs))
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // remove_at_c
    //////////////////////////////////////////////////////////////////////////
    template <detail::std::size_t n>
    struct _remove_at_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return remove_at(size_t<n>, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // reverse
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct reverse_impl : reverse_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct reverse_impl<S, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return foldl(detail::std::forward<Xs>(xs), nil<S>(), flip(prepend));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // scanl
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct scanl_impl : scanl_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct scanl_impl<S, when<condition>> {
        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State state, F f) {
            return eval_if(is_empty(xs),
                [=](auto _) { return lift<S>(state); },
                [=](auto _) {
                    return prepend(state,
                        apply(_(tail)(xs), f(state, _(head)(xs)), f));
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // scanl1
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct scanl1_impl : scanl1_impl<S, when<true>> { };

    namespace sequence_detail {
        struct scanl1_helper {
            template <typename Xs, typename F, typename Id>
            constexpr decltype(auto) operator()(Xs&& xs, F&& f, Id const& _) const {
                //! @todo We need a way to extract the head of an Iterable
                //! and get its tail at the same time. It would allow us to
                //! use perfect forwarding here.
                return hana::scanl(_(tail)(xs), _(head)(xs),
                        detail::std::forward<F>(f));
            }
        };
    }

    template <typename S, bool condition>
    struct scanl1_impl<S, when<condition>> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs&& xs, F&& f) {
            decltype(auto) done = hana::is_empty(xs);
            return hana::eval_if(detail::std::forward<decltype(done)>(done),
                hana::always(nil<S>()),
                hana::partial(sequence_detail::scanl1_helper{},
                    detail::std::forward<Xs>(xs), detail::std::forward<F>(f))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // scanr
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct scanr_impl : scanr_impl<Xs, when<true>> { };

    template <typename S, bool condition>
    struct scanr_impl<S, when<condition>> {
        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State state, F f) {
            return eval_if(is_empty(xs),
                [=](auto _) { return lift<S>(state); },
                [=](auto _) {
                    auto rest = apply(_(tail)(xs), state, f);
                    return prepend(f(_(head)(xs), head(rest)), rest);
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // scanr1
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct scanr1_impl : scanr1_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct scanr1_impl<S, when<condition>> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return eval_if(is_empty(xs),
                always(nil<L>()),
                [=](auto _) {
                    auto y = _(head)(xs);
                    auto ys = _(tail)(xs);
                    return eval_if(is_empty(ys),
                        [=](auto _) { return lift<S>(y); },
                        [=](auto _) {
                            auto rest = apply(ys, _(f));
                            return prepend(f(y, head(rest)), rest);
                        }
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // slice
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct slice_impl : slice_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct slice_impl<S, when<condition>> {
        template <typename Xs, typename From, typename To>
        static constexpr decltype(auto) apply(Xs&& xs, From const& from, To const& to) {
            return hana::take(hana::minus(to, from),
                    hana::drop(from, detail::std::forward<Xs>(xs)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // slice_c
    //////////////////////////////////////////////////////////////////////////
    template <detail::std::size_t from, detail::std::size_t to>
    struct _slice_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return hana::slice(detail::std::forward<Xs>(xs),
                    size_t<from>, size_t<to>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // sort_by
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct sort_by_impl : sort_by_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct sort_by_impl<S, when<condition>> {
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

    //////////////////////////////////////////////////////////////////////////
    // sort
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct sort_impl : sort_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct sort_impl<S, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::sort_by(less, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // span
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct span_impl : span_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct span_impl<S, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                [=](auto _) { return pair(nil<S>(), nil<S>()); },
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
                            return _(pair)(nil<S>(), xs);
                        }
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct take_impl : take_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct take_impl<S, when<condition>> {
        template <typename N, typename Xs>
        static constexpr auto apply(N n, Xs xs) {
            return eval_if(or_(is_empty(xs), equal(n, int_<0>)),
                always(nil<S>()),
                [=](auto _) {
                    return prepend(_(head)(xs), apply(minus(n, int_<1>), _(tail)(xs)));
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take_c
    //////////////////////////////////////////////////////////////////////////
    template <detail::std::size_t n>
    struct _take_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return hana::take(size_t<n>, detail::std::forward<Xs>(xs));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take_until
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct take_until_impl : take_until_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct take_until_impl<S, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::take_while(
                detail::std::forward<Xs>(xs),
                hana::compose(not_, detail::std::forward<Pred>(pred))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take_while
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct take_while_impl : take_while_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct take_while_impl<S, when<condition>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                always(nil<S>()),
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) {
                            return cons(
                                _(head)(xs),
                                apply(_(tail)(xs), pred)
                            );
                        },
                        always(nil<S>())
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unfoldl
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct unfoldl_impl : unfoldl_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct unfoldl_impl<S, when<condition>> {
        template <typename F, typename Init>
        static constexpr auto apply(F f, Init init) {
            auto g = [=](auto b_a) {
                return append(apply(f, first(b_a)), second(b_a));
            };
            return maybe(nil<S>(), g, f(init));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unfoldr
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct unfoldr_impl : unfoldr_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct unfoldr_impl<S, when<condition>> {
        template <typename F, typename Init>
        static constexpr auto apply(F f, Init init) {
            auto g = [=](auto a_b) {
                return cons(first(a_b), apply(f, second(a_b)));
            };
            return maybe(nil<S>(), g, f(init));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unzip
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct unzip_impl : unzip_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct unzip_impl<S, when<condition>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::unpack(detail::std::forward<Xs>(xs), zip); }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct zip_impl : zip_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_impl<S, when<condition>> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs)
        { return hana::zip_with(make<S>, detail::std::forward<Xs>(xs)...); }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip_with
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct zip_with_impl : zip_with_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_with_impl<S, when<condition>> {
        template <typename F, typename ...Xss>
        static constexpr auto apply(F f, Xss ...xss) {
            return eval_if(or_(is_empty(xss)...),
                [](auto _) { return nil<S>(); },
                [=](auto _) {
                    return cons(
                        f(_(head)(xss)...),
                        apply(f, _(tail)(xss)...)
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence::equal_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct Sequence::equal_impl {
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

    //////////////////////////////////////////////////////////////////////////
    // Sequence::traverse_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct Sequence::traverse_impl {
        struct helper {
            template <typename F, typename X, typename Ys>
            constexpr decltype(auto) operator()(F&& f, X&& x, Ys&& ys) const {
                return hana::ap(
                    hana::transform(
                        detail::std::forward<F>(f)(detail::std::forward<X>(x)),
                        curry<2>(prepend)
                    ),
                    detail::std::forward<Ys>(ys)
                );
            }
        };

        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return foldr(detail::std::forward<Xs>(xs), lift<A>(nil<T>()),
                hana::partial(helper{}, detail::std::forward<F>(f)));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEQUENCE_HPP
