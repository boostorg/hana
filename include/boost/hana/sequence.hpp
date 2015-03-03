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

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/traversable.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    namespace sequence_detail {
        //! @todo Find a way to make it so that `lazy` interoperates properly
        //! with `eval_if`, without having to do this hack.
        struct _lazily {
            template <typename F, typename Id>
            constexpr decltype(auto) operator()(F&& f, Id _) const
            { return hana::eval(detail::std::forward<F>(f)); }
        };

        template <typename F, typename ...X>
        constexpr decltype(auto) lazily(F&& f, X&& ...x) {
            return hana::partial(_lazily{},
                hana::lazy(detail::std::forward<F>(f))(
                    detail::std::forward<X>(x)...));
        }

        struct pairwise {
            template <typename F, typename G, typename P>
            constexpr decltype(auto) operator()(F&& f, G&& g, P&& p) const {
                return hana::pair(
                    detail::std::forward<F>(f)(hana::first(detail::std::forward<P>(p))),
                    detail::std::forward<G>(g)(hana::second(detail::std::forward<P>(p)))
                );
            }
        };

        struct _for {
            template <typename Init, typename Pred, typename Incr, typename F, typename State>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr, F&& f, State&& state) const {
                return hana::second(
                    hana::while_(
                        hana::compose(detail::std::forward<Pred>(pred), first),
                        hana::pair(detail::std::forward<Init>(init),
                                   detail::std::forward<State>(state)),
                        hana::partial(pairwise{},
                            detail::std::forward<Incr>(incr),
                            detail::std::forward<F>(f)
                        )
                    )
                );
            }

            template <typename Init, typename Pred, typename Incr>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr) const {
                return curry<2>(hana::partial(*this,
                    detail::std::forward<Init>(init),
                    detail::std::forward<Pred>(pred),
                    detail::std::forward<Incr>(incr)
                ));
            }
        };

        constexpr _for for_{};

        struct pairwise2 {
            template <typename F, typename G, typename P>
            constexpr decltype(auto) operator()(F&& f, G&& g, P&& p) const {
                return hana::pair(
                    detail::std::forward<F>(f)(hana::first(p)),
                    detail::std::forward<G>(g)(
                        hana::first(p),
                        hana::second(detail::std::forward<P>(p)))
                );
            }
        };

        struct _for2 {
            template <typename Init, typename Pred, typename Incr, typename F, typename State>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr, F&& f, State&& state) const {
                return hana::second(
                    hana::while_(
                        hana::compose(detail::std::forward<Pred>(pred), first),
                        hana::pair(detail::std::forward<Init>(init),
                                   detail::std::forward<State>(state)),
                        hana::partial(pairwise2{},
                            detail::std::forward<Incr>(incr),
                            detail::std::forward<F>(f)
                        )
                    )
                );
            }

            template <typename Init, typename Pred, typename Incr>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr) const {
                return curry<2>(hana::partial(*this,
                    detail::std::forward<Init>(init),
                    detail::std::forward<Pred>(pred),
                    detail::std::forward<Incr>(incr)
                ));
            }
        };

        constexpr _for2 for2_{};
    } // end namespace sequence_detail

    //////////////////////////////////////////////////////////////////////////
    // group_by
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct group_by_impl : group_by_impl<S, when<true>> { };

    namespace sequence_detail {
        struct group_by_loop {
            template <typename Pred, typename Grouped, typename Xs>
            constexpr decltype(auto)
            operator()(Pred&& pred, Grouped&& grouped, Xs&& xs) const {
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
    struct group_by_impl<S, when<condition>> : default_ {
        template <typename Pred, typename Xs>
        static constexpr decltype(auto) apply(Pred&& pred, Xs&& xs) {
            return hana::first(hana::until(hana::compose(is_empty, second),
                hana::pair(empty<S>(), detail::std::forward<Xs>(xs)),
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
    struct group_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::group_by(equal, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // init
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct init_impl : init_impl<S, when<true>> { };

    namespace sequence_detail {
        struct init_helper {
            template <typename Xs, typename Init>
            constexpr decltype(auto) operator()(Xs&& xs, Init&& init) const {
                return hana::append(detail::std::forward<Init>(init),
                                    hana::head(detail::std::forward<Xs>(xs)));
            }
        };
    }

    template <typename S, bool condition>
    struct init_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return sequence_detail::for2_(
                detail::std::forward<Xs>(xs),
                hana::compose(not_, is_empty, tail),
                tail
            )(sequence_detail::init_helper{})(empty<S>());
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // intersperse
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct intersperse_impl : intersperse_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct intersperse_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            return hana::eval_if(hana::is_empty(xs),
                [&xs](auto _) -> decltype(auto) {
                    return hana::id(detail::std::forward<Xs>(xs));
                },
                [&xs, &z](auto _) -> decltype(auto) {
                    return hana::prepend(
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

    namespace sequence_detail {
        struct append_first {
            template <typename Parts, typename X>
            constexpr decltype(auto) operator()(Parts&& parts, X&& x) const {
                return hana::pair(
                    hana::append(hana::first(detail::std::forward<Parts>(parts)),
                                 detail::std::forward<X>(x)),
                    hana::second(detail::std::forward<Parts>(parts))
                );
            }
        };

        struct append_second {
            template <typename Parts, typename X>
            constexpr decltype(auto) operator()(Parts&& parts, X&& x) const {
                return hana::pair(
                    hana::first(detail::std::forward<Parts>(parts)),
                    hana::append(hana::second(detail::std::forward<Parts>(parts)),
                                 detail::std::forward<X>(x))
                );
            }
        };

        //! @todo How could we avoid copying `parts` and `x` to both branches?
        struct partition_helper {
            template <typename Pred, typename Parts, typename X>
            constexpr decltype(auto)
            operator()(Pred&& pred, Parts&& parts, X&& x) const {
                return hana::eval_if(detail::std::forward<Pred>(pred)(x),
                    sequence_detail::lazily(sequence_detail::append_first{}, parts, x),
                    sequence_detail::lazily(sequence_detail::append_second{}, parts, x)
                );
            }
        };
    }

    template <typename S, bool condition>
    struct partition_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::foldl(
                detail::std::forward<Xs>(xs),
                hana::pair(empty<S>(), empty<S>()),
                hana::partial(sequence_detail::partition_helper{},
                              detail::std::forward<Pred>(pred))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // permutations
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct permutations_impl : permutations_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct permutations_impl<S, when<condition>> : default_ {
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
                [](auto _) { return lift<S>(empty<S>()); },
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
    struct remove_at_impl<S, when<condition>> : default_ {
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
        { return hana::remove_at(size_t<n>, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // reverse
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct reverse_impl : reverse_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct reverse_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::foldl(detail::std::forward<Xs>(xs), empty<S>(),
                                                        hana::flip(prepend));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // scanl
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct scanl_impl : scanl_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct scanl_impl<S, when<condition>> : default_ {
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
            constexpr decltype(auto)
            operator()(Xs&& xs, F&& f, Id const& _) const {
                //! @todo We need a way to extract the head of an Iterable
                //! and get its tail at the same time. It would allow us to
                //! use perfect forwarding here.
                return hana::scanl(_(tail)(xs), _(head)(xs),
                        detail::std::forward<F>(f));
            }
        };
    }

    template <typename S, bool condition>
    struct scanl1_impl<S, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs&& xs, F&& f) {
            decltype(auto) done = hana::is_empty(xs);
            return hana::eval_if(detail::std::forward<decltype(done)>(done),
                hana::always(empty<S>()),
                hana::partial(sequence_detail::scanl1_helper{},
                    detail::std::forward<Xs>(xs), detail::std::forward<F>(f))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // scanr
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct scanr_impl : scanr_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct scanr_impl<S, when<condition>> : default_ {
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
    struct scanr1_impl<S, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return eval_if(is_empty(xs),
                always(empty<S>()),
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
    struct slice_impl<S, when<condition>> : default_ {
        template <typename Xs, typename From, typename To>
        static constexpr decltype(auto)
        apply(Xs&& xs, From const& from, To const& to) {
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
    struct sort_by_impl<S, when<condition>> : default_ {
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
                            auto parts = partition(rest, [=](auto x) {
                                return pred(x, pivot);
                            });
                            return concat(
                                apply(pred, first(parts)),
                                prepend(pivot, apply(pred, second(parts)))
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
    struct sort_impl<S, when<condition>> : default_ {
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
    struct span_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                [=](auto _) { return pair(empty<S>(), empty<S>()); },
                [=](auto _) {
                    auto x = _(head)(xs);
                    auto xs_ = _(tail)(xs);
                    return eval_if(pred(x),
                        [=](auto _) {
                            auto ys_zs = apply(xs_, _(pred));
                            auto ys = first(ys_zs);
                            auto zs = second(ys_zs);
                            return pair(prepend(x, ys), zs);
                        },
                        [=](auto _) {
                            return _(pair)(empty<S>(), xs);
                        }
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take.exactly
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct take_exactly_impl : take_exactly_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct take_exactly_impl<S, when<condition>> : default_ {
        template <typename N, typename Xs>
        static constexpr auto apply(N n, Xs xs) {
            return hana::eval_if(hana::equal(n, size_t<0>),
                hana::always(empty<S>()),
                [=](auto _) {
                    return hana::prepend(_(head)(xs),
                                         apply(_(pred)(n), _(tail)(xs)));
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take.at_most
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct take_at_most_impl : take_at_most_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct take_at_most_impl<S, when<condition>> : default_ {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N n, Xs xs) {
            return hana::eval_if(
                hana::or_(hana::equal(n, size_t<0>), hana::is_empty(xs)),
                hana::always(empty<S>()),
                [=](auto _) {
                    return hana::prepend(_(head)(xs),
                                         apply(_(pred)(n), _(tail)(xs)));
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
    struct take_until_impl<S, when<condition>> : default_ {
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

    namespace sequence_detail {
        struct take_while_helper {
            template <typename Xs, typename State>
            constexpr decltype(auto) operator()(Xs&& xs, State&& state) const {
                return hana::append(detail::std::forward<State>(state),
                                    hana::head(detail::std::forward<Xs>(xs)));
            }
        };

        struct take_while_predicate {
            template <typename Pred, typename Xs>
            static constexpr decltype(auto)
            helper(Pred&& pred, Xs&& xs, bool cond) {
                return cond ? false
                            : detail::std::forward<Pred>(pred)(
                                hana::head(detail::std::forward<Xs>(xs)));
            }

            template <typename Pred, typename Xs>
            static constexpr decltype(auto)
            helper(Pred&& pred, Xs&& xs, detail::std::false_type) {
                return detail::std::forward<Pred>(pred)(
                        hana::head(detail::std::forward<Xs>(xs)));
            }

            template <typename Pred, typename Xs>
            static constexpr decltype(auto)
            helper(Pred&& pred, Xs&& xs, detail::std::true_type)
            { return false_; }

            template <typename Pred, typename Xs>
            constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
                return helper(
                    detail::std::forward<Pred>(pred),
                    detail::std::forward<Xs>(xs),
                    hana::if_(hana::is_empty(xs),
                        detail::std::true_type{},
                        detail::std::false_type{}));
            }
        };
    }

    template <typename S, bool condition>
    struct take_while_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return sequence_detail::for2_(
                detail::std::forward<Xs>(xs),
                hana::partial(sequence_detail::take_while_predicate{},
                              detail::std::forward<Pred>(pred)),
                tail
            )(sequence_detail::take_while_helper{})(empty<S>());
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unfoldl
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct unfoldl_impl : unfoldl_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct unfoldl_impl<S, when<condition>> : default_ {
        struct unfoldl_helper {
            template <typename F, typename P>
            constexpr decltype(auto) operator()(F&& f, P&& p) const {
                return hana::append(
                    unfoldl_impl::apply(detail::std::forward<F>(f),
                                  hana::first(detail::std::forward<P>(p))),
                    hana::second(detail::std::forward<P>(p))
                );
            }
        };

        template <typename F, typename Init>
        static constexpr decltype(auto) apply(F&& f, Init&& init) {
            return hana::maybe(empty<S>(),
                hana::partial(unfoldl_helper{}, f),
                f(detail::std::forward<Init>(init))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unfoldr
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct unfoldr_impl : unfoldr_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct unfoldr_impl<S, when<condition>> : default_ {
        struct unfoldr_helper {
            template <typename F, typename P>
            constexpr decltype(auto) operator()(F&& f, P&& p) const {
                return hana::prepend(
                    hana::first(detail::std::forward<P>(p)),
                    unfoldr_impl::apply(detail::std::forward<F>(f),
                        hana::second(detail::std::forward<P>(p)))
                );
            }
        };

        template <typename F, typename Init>
        static constexpr decltype(auto) apply(F&& f, Init&& init) {
            return hana::maybe(empty<S>(),
                hana::partial(unfoldr_helper{}, f),
                f(detail::std::forward<Init>(init))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unzip
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct unzip_impl : unzip_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct unzip_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::unpack(detail::std::forward<Xs>(xs), zip); }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.shortest.with
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct zip_shortest_with_impl : zip_shortest_with_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_shortest_with_impl<S, when<condition>> : default_ {
        template <typename F, typename ...Xs>
        static constexpr decltype(auto) apply(F&& f, Xs&& ...xs) {
            auto min = hana::minimum(hana::make<Tuple>(hana::length(xs)...));
            return zip.unsafe.with(detail::std::forward<F>(f),
                hana::take(min, detail::std::forward<Xs>(xs))...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.shortest
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct zip_shortest_impl : zip_shortest_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_shortest_impl<S, when<condition>> : default_ {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return zip.shortest.with(make<Tuple>, detail::std::forward<Xs>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.unsafe.with
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct zip_unsafe_with_impl : zip_unsafe_with_impl<S, when<true>> { };

    namespace sequence_detail {
        struct zip_unsafe_with_helper {
            template <typename F, typename ...Xs>
            constexpr decltype(auto) operator()(F&& f, Xs&& ...xs) const {
                return hana::prepend(
                    f(hana::head(xs)...),
                    zip.unsafe.with(f, hana::tail(xs)...)
                );
            }
        };
    }

    template <typename S, bool condition>
    struct zip_unsafe_with_impl<S, when<condition>> : default_ {
        template <typename F, typename Xs, typename ...Ys>
        static constexpr decltype(auto) apply(F&& f, Xs&& xs, Ys&& ...ys) {
            auto done = hana::is_empty(xs);
            return hana::eval_if(done,
                hana::always(empty<S>()),
                sequence_detail::lazily(sequence_detail::zip_unsafe_with_helper{},
                    detail::std::forward<F>(f),
                    detail::std::forward<Xs>(xs),
                    detail::std::forward<Ys>(ys)...
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.unsafe
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct zip_unsafe_impl : zip_unsafe_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_unsafe_impl<S, when<condition>> : default_ {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return zip.unsafe.with(make<Tuple>, detail::std::forward<Xs>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct make_impl<S, when<models<Sequence(S)>{}>> {
        template <typename ...X>
        static constexpr decltype(auto) apply(X&& ...x) {
            return detail::variadic::foldr(prepend, empty<S>(),
                    detail::std::forward<X>(x)...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct Sequence::equal_impl {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto)
        equal_helper(Xs const& xs, Ys const& ys, detail::std::true_type) {
            return hana::and_(hana::is_empty(xs), hana::is_empty(ys));
        }

        template <typename Xs, typename Ys>
        static constexpr decltype(auto)
        equal_helper(Xs const& xs, Ys const& ys, detail::std::false_type) {
            return hana::and_(
                hana::equal(hana::head(xs), hana::head(ys)),
                apply(hana::tail(xs), hana::tail(ys))
            );
        }

        template <typename Xs, typename Ys>
        static constexpr decltype(auto)
        equal_helper(Xs const& xs, Ys const& ys, bool cond) {
            return cond
                ? hana::and_(hana::is_empty(xs), hana::is_empty(ys))
                : hana::and_(
                    hana::equal(hana::head(xs), hana::head(ys)),
                    apply(hana::tail(xs), hana::tail(ys))
                  );
        }

        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs const& xs, Ys const& ys) {
            auto done = hana::or_(hana::is_empty(xs), hana::is_empty(ys));
            return equal_helper(xs, ys, hana::if_(done,
                                            detail::std::true_type{},
                                            detail::std::false_type{}));
        }
    };

    template <typename T, typename U>
    struct equal_impl<T, U, when<models<Sequence(T)>{} && models<Sequence(U)>{}>>
        : Sequence::equal_impl<T, U>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct Sequence::less_impl {
        using yes = detail::std::true_type;
        using no = detail::std::false_type;

        template <typename Xs, typename Ys>
        static constexpr auto less_helper(Xs const&, Ys const&, yes, yes)
        { return false_; }

        template <typename Xs, typename Ys>
        static constexpr auto less_helper(Xs const&, Ys const&, yes, no)
        { return true_; }

        template <typename Xs, typename Ys>
        static constexpr auto less_helper(Xs const&, Ys const&, no, yes)
        { return false_; }

        template <typename Xs, typename Ys>
        static constexpr auto less_helper(Xs const& xs, Ys const& ys, no, no) {
            return hana::or_(
                hana::less(hana::head(xs), hana::head(ys)),
                hana::and_(
                    hana::equal(hana::head(xs), hana::head(ys)),
                    apply(hana::tail(xs), hana::tail(ys))
                )
            );
        }

        template <typename Xs, typename Ys>
        static constexpr bool
        less_helper(Xs const& xs, Ys const& ys, bool e_xs, bool e_ys) {
                 if (e_xs && e_ys) return false;
            else if (e_xs && !e_ys) return true;
            else if (!e_xs && e_ys) return false;

            return hana::or_(
                hana::less(hana::head(xs), hana::head(ys)),
                hana::and_(
                    hana::equal(hana::head(xs), hana::head(ys)),
                    apply(hana::tail(xs), hana::tail(ys))
                )
            );
        }

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs const& xs, Ys const& ys) {
            return less_helper(xs, ys,
                hana::if_(hana::is_empty(xs), yes{}, no{}),
                hana::if_(hana::is_empty(ys), yes{}, no{}));
        }
    };

    template <typename T, typename U>
    struct less_impl<T, U, when<models<Sequence(T)>{} && models<Sequence(U)>{}>>
        : Sequence::less_impl<T, U>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Functor
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::transform_impl {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::foldr(detail::std::forward<Xs>(xs), empty<S>(),
                        hana::compose(prepend, detail::std::forward<F>(f)));
        }
    };

    template <typename S>
    struct transform_impl<S, when<models<Sequence(S)>{}>>
        : Sequence::transform_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Applicative
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::lift_impl {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::prepend(detail::std::forward<X>(x), empty<S>()); }
    };

    template <typename S>
    struct ap_impl<S, when<models<Sequence(S)>{}>>
        : Monad::ap_impl<S>
    { };

    template <typename S>
    struct lift_impl<S, when<models<Sequence(S)>{}>>
        : Sequence::lift_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Monad
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::flatten_impl {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::foldr(detail::std::forward<Xs>(xs), empty<S>(), concat); }
    };

    template <typename S>
    struct flatten_impl<S, when<models<Sequence(S)>{}>>
        : Sequence::flatten_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Partial model of MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::concat_impl {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::foldr(detail::std::forward<Xs>(xs),
                               detail::std::forward<Ys>(ys), prepend);
        }
    };

    template <typename S>
    struct concat_impl<S, when<models<Sequence(S)>{}>>
        : Sequence::concat_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct foldl_impl<S, when<models<Sequence(S)>{}>>
        : Iterable::foldl_impl<S>
    { };

    template <typename S>
    struct foldr_impl<S, when<models<Sequence(S)>{}>>
        : Iterable::foldr_impl<S>
    { };

    template <typename S>
    struct foldl1_impl<S, when<models<Sequence(S)>{}>>
        : Iterable::foldl1_impl<S>
    { };

    template <typename S>
    struct foldr1_impl<S, when<models<Sequence(S)>{}>>
        : Iterable::foldr1_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct find_impl<S, when<models<Sequence(S)>{}>>
        : Iterable::find_impl<S>
    { };

    template <typename S>
    struct any_of_impl<S, when<models<Sequence(S)>{}>>
        : Iterable::any_of_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Traversable
    //////////////////////////////////////////////////////////////////////////
    namespace sequence_detail {
        struct traverse_helper {
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
    }

    template <typename S>
    struct Sequence::traverse_impl {
        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::foldr(detail::std::forward<Xs>(xs), lift<A>(empty<S>()),
                hana::partial(sequence_detail::traverse_helper{},
                              detail::std::forward<F>(f)));
        }
    };

    template <typename S>
    struct traverse_impl<S, when<models<Sequence(S)>{}>>
        : Sequence::traverse_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic Foldable -> Sequence conversion
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename F>
    struct to_impl<S, F, when<models<Sequence(S)>{} && models<Foldable(F)>{}>>
        : embedding<models<Sequence(F)>{}>
    {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::foldr(detail::std::forward<Xs>(xs), empty<S>(), prepend); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEQUENCE_HPP
