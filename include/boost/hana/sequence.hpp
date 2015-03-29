/*!
@file
Defines `boost::hana::Sequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEQUENCE_HPP
#define BOOST_HANA_SEQUENCE_HPP

#include <boost/hana/fwd/sequence.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
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
#include <boost/hana/searchable.hpp>
#include <boost/hana/traversable.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    namespace sequence_detail {
        struct pairwise {
            template <typename F, typename G, typename P>
            constexpr decltype(auto) operator()(F&& f, G&& g, P&& p) const {
                return hana::pair(
                    static_cast<F&&>(f)(hana::first(static_cast<P&&>(p))),
                    static_cast<G&&>(g)(hana::second(static_cast<P&&>(p)))
                );
            }
        };

        struct _for {
            template <typename Init, typename Pred, typename Incr, typename F, typename State>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr, F&& f, State&& state) const {
                return hana::second(
                    hana::while_(
                        hana::compose(static_cast<Pred&&>(pred), first),
                        hana::pair(static_cast<Init&&>(init),
                                   static_cast<State&&>(state)),
                        hana::partial(pairwise{},
                            static_cast<Incr&&>(incr),
                            static_cast<F&&>(f)
                        )
                    )
                );
            }

            template <typename Init, typename Pred, typename Incr>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr) const {
                return curry<2>(hana::partial(*this,
                    static_cast<Init&&>(init),
                    static_cast<Pred&&>(pred),
                    static_cast<Incr&&>(incr)
                ));
            }
        };

        constexpr _for for_{};

        struct pairwise2 {
            template <typename F, typename G, typename P>
            constexpr decltype(auto) operator()(F&& f, G&& g, P&& p) const {
                return hana::pair(
                    static_cast<F&&>(f)(hana::first(p)),
                    static_cast<G&&>(g)(
                        hana::first(p),
                        hana::second(static_cast<P&&>(p)))
                );
            }
        };

        struct _for2 {
            template <typename Init, typename Pred, typename Incr, typename F, typename State>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr, F&& f, State&& state) const {
                return hana::second(
                    hana::while_(
                        hana::compose(static_cast<Pred&&>(pred), first),
                        hana::pair(static_cast<Init&&>(init),
                                   static_cast<State&&>(state)),
                        hana::partial(pairwise2{},
                            static_cast<Incr&&>(incr),
                            static_cast<F&&>(f)
                        )
                    )
                );
            }

            template <typename Init, typename Pred, typename Incr>
            constexpr decltype(auto)
            operator()(Init&& init, Pred&& pred, Incr&& incr) const {
                return curry<2>(hana::partial(*this,
                    static_cast<Init&&>(init),
                    static_cast<Pred&&>(pred),
                    static_cast<Incr&&>(incr)
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
                decltype(auto) tmp = hana::span(static_cast<Xs&&>(xs),
                    hana::partial(static_cast<Pred&&>(pred),
                                  detail::std::forward<decltype(x)>(x)));
                decltype(auto) spn = hana::first(detail::std::forward<decltype(tmp)>(tmp));
                decltype(auto) rest = hana::second(detail::std::forward<decltype(tmp)>(tmp));
                return hana::pair(
                    hana::append(static_cast<Grouped&&>(grouped),
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
                hana::pair(empty<S>(), static_cast<Xs&&>(xs)),
                hana::fuse(hana::partial(sequence_detail::group_by_loop{},
                                         static_cast<Pred&&>(pred)))
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
        { return hana::group_by(equal, static_cast<Xs&&>(xs)); }
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
                return hana::append(static_cast<Init&&>(init),
                                    hana::head(static_cast<Xs&&>(xs)));
            }
        };
    }

    template <typename S, bool condition>
    struct init_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return sequence_detail::for2_(
                static_cast<Xs&&>(xs),
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
        struct intersperse_helper {
            template <typename Xs, typename Z>
            constexpr decltype(auto) operator()(Xs&& xs, Z&& z) const {
                return hana::prepend(
                    hana::head(xs),
                    hana::prefix(static_cast<Z&&>(z), hana::tail(xs))
                );
            }
        };

        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(xs),
                hana::lazy(intersperse_helper{})(xs, z)
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
                    hana::append(hana::first(static_cast<Parts&&>(parts)),
                                 static_cast<X&&>(x)),
                    hana::second(static_cast<Parts&&>(parts))
                );
            }
        };

        struct append_second {
            template <typename Parts, typename X>
            constexpr decltype(auto) operator()(Parts&& parts, X&& x) const {
                return hana::pair(
                    hana::first(static_cast<Parts&&>(parts)),
                    hana::append(hana::second(static_cast<Parts&&>(parts)),
                                 static_cast<X&&>(x))
                );
            }
        };

        //! @todo How could we avoid copying `parts` and `x` to both branches?
        struct partition_helper {
            template <typename Pred, typename Parts, typename X>
            constexpr decltype(auto)
            operator()(Pred&& pred, Parts&& parts, X&& x) const {
                return hana::eval_if(static_cast<Pred&&>(pred)(x),
                    hana::lazy(sequence_detail::append_first{})(parts, x),
                    hana::lazy(sequence_detail::append_second{})(parts, x)
                );
            }
        };
    }

    template <typename S, bool condition>
    struct partition_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::foldl(
                static_cast<Xs&&>(xs),
                hana::pair(empty<S>(), empty<S>()),
                hana::partial(sequence_detail::partition_helper{},
                              static_cast<Pred&&>(pred))
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
        struct insertions2 {
            template <typename X, typename Xs>
            constexpr decltype(auto) operator()(X&& x, Xs&& l) const {
                auto y = hana::head(l);
                auto ys = hana::tail(l);
                return hana::prepend(
                    hana::prepend(x, l),
                    hana::transform(insertions{}(x, ys),
                                    hana::partial(prepend, y))
                );
            }
        };

        struct insertions {
            template <typename X, typename Xs>
            constexpr decltype(auto) operator()(X&& x, Xs&& l) const {
                return hana::eval_if(hana::is_empty(l),
                    hana::lazy(hana::compose(lift<S>, lift<S>))(x),
                    hana::lazy(insertions2{})(x, l)
                );
            }
        };

        struct permutations_helper {
            template <typename Xs>
            constexpr decltype(auto) operator()(Xs&& xs) const {
                return hana::bind(
                    permutations_impl::apply(hana::tail(xs)),
                    hana::partial(insertions{}, hana::head(xs))
                );
            }
        };

        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(lift<S>)(empty<S>()),
                hana::lazy(permutations_helper{})(xs)
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
        struct remove_at_helper {
            template <typename N, typename Xs>
            constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
                return hana::prepend(
                    hana::head(xs),
                    remove_at_impl::apply(hana::pred(n), hana::tail(xs))
                );
            }
        };

        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N&& n, Xs&& xs) {
            using I = typename datatype<N>::type;
            return hana::eval_if(hana::equal(n, zero<I>()),
                hana::lazy(hana::tail)(xs),
                hana::lazy(remove_at_helper{})(n, xs)
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
        { return hana::remove_at(size_t<n>, static_cast<Xs&&>(xs)); }
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
            return hana::foldl(static_cast<Xs&&>(xs), empty<S>(),
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
        struct scanl_helper {
            template <typename Xs, typename State, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
                return hana::prepend(state,
                    scanl_impl::apply(hana::tail(xs), f(state, hana::head(xs)), f));
            }
        };

        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State state, F f) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(hana::lift<S>(state)),
                hana::lazy(scanl_helper{})(xs, state, f)
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
            template <typename Xs, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
                //! @todo We need a way to extract the head of an Iterable
                //! and get its tail at the same time. It would allow us to
                //! use perfect forwarding here.
                return hana::scanl(hana::tail(xs), hana::head(xs),
                        static_cast<F&&>(f));
            }
        };
    }

    template <typename S, bool condition>
    struct scanl1_impl<S, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs&& xs, F&& f) {
            decltype(auto) done = hana::is_empty(xs);
            return hana::eval_if(detail::std::forward<decltype(done)>(done),
                hana::lazy(empty<S>()),
                hana::lazy(sequence_detail::scanl1_helper{})(
                    static_cast<Xs&&>(xs), static_cast<F&&>(f)
                )
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
        struct scanr_helper {
            template <typename Xs, typename State, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
                auto rest = scanr_impl::apply(hana::tail(xs), state, f);
                return hana::prepend(f(hana::head(xs), hana::head(rest)), rest);
            }
        };

        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State state, F f) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(hana::lift<S>(state)),
                hana::lazy(scanr_helper{})(xs, state, f)
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
        struct scanr1_helper2 {
            template <typename Y, typename Ys, typename F>
            constexpr decltype(auto) operator()(Y&& y, Ys&& ys, F&& f) const {
                auto rest = scanr1_impl::apply(ys, f);
                return hana::prepend(f(y, hana::head(rest)), rest);
            }
        };

        struct scanr1_helper1 {
            template <typename Xs, typename F>
            constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
                auto y = hana::head(xs);
                auto ys = hana::tail(xs);
                return hana::eval_if(hana::is_empty(ys),
                    hana::lazy(hana::lift<S>(y)),
                    hana::lazy(scanr1_helper2{})(y, ys, f)
                );
            }
        };

        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(empty<S>()),
                hana::lazy(scanr1_helper1{})(xs, f)
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
                    hana::drop(from, static_cast<Xs&&>(xs)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // slice_c
    //////////////////////////////////////////////////////////////////////////
    template <detail::std::size_t from, detail::std::size_t to>
    struct _slice_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return hana::slice(static_cast<Xs&&>(xs),
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
        struct sort_by_helper2 {
            template <typename Pred, typename Xs>
            constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
                auto pivot = hana::head(xs);
                auto rest = hana::tail(xs);
                auto parts = hana::partition(rest,
                                    hana::partial(hana::flip(pred), pivot));
                return hana::concat(
                    sort_by_impl::apply(pred, hana::first(parts)),
                    hana::prepend(pivot,
                            sort_by_impl::apply(pred, hana::second(parts)))
                );
            }
        };

        struct sort_by_helper1 {
            template <typename Pred, typename Xs>
            constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
                return hana::eval_if(hana::is_empty(hana::tail(xs)),
                    hana::lazy(xs),
                    hana::lazy(sort_by_helper2{})(pred, xs)
                );
            }
        };

        template <typename Pred, typename Xs>
        static constexpr auto apply(Pred pred, Xs xs) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(xs),
                hana::lazy(sort_by_helper1{})(pred, xs)
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
        { return hana::sort_by(less, static_cast<Xs&&>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // span
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct span_impl : span_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct span_impl<S, when<condition>> : default_ {
        struct span_helper2 {
            template <typename Xs, typename Pred>
            constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                auto ys_zs = span_impl::apply(hana::tail(xs), pred);
                auto ys = hana::first(ys_zs);
                auto zs = hana::second(ys_zs);
                return hana::make_pair(hana::prepend(hana::head(xs), ys), zs);
            }
        };

        struct span_helper1 {
            template <typename Xs, typename Pred>
            constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                return hana::eval_if(pred(hana::head(xs)),
                    hana::lazy(span_helper2{})(xs, pred),
                    hana::lazy(make_pair)(empty<S>(), xs)
                );
            }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(make_pair)(empty<S>(), empty<S>()),
                hana::lazy(span_helper1{})(xs, pred)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // take.exactly
    //////////////////////////////////////////////////////////////////////////
    namespace sequence_detail {
        template <typename TakeHelper>
        struct take_helper {
            template <typename N, typename Xs>
            constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
                return hana::prepend(hana::head(xs),
                        TakeHelper::apply(
                            hana::pred(static_cast<N&&>(n)),
                            hana::tail(xs)));
            }
        };
    }

    template <typename S, typename>
    struct take_exactly_impl : take_exactly_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct take_exactly_impl<S, when<condition>> : default_ {
        template <typename N, typename Xs>
        static constexpr auto apply(N n, Xs xs) {
            return hana::eval_if(hana::equal(n, size_t<0>),
                hana::lazy(empty<S>()),
                hana::lazy(sequence_detail::take_helper<take_exactly_impl>{})(n, xs)
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
                hana::lazy(empty<S>()),
                hana::lazy(sequence_detail::take_helper<take_at_most_impl>{})(n, xs)
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
            return hana::take(size_t<n>, static_cast<Xs&&>(xs));
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
                static_cast<Xs&&>(xs),
                hana::compose(not_, static_cast<Pred&&>(pred))
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
                return hana::append(static_cast<State&&>(state),
                                    hana::head(static_cast<Xs&&>(xs)));
            }
        };

        struct take_while_predicate {
            template <typename Pred, typename Xs>
            static constexpr decltype(auto)
            helper(Pred&& pred, Xs&& xs, bool cond) {
                return cond ? false
                            : static_cast<Pred&&>(pred)(
                                hana::head(static_cast<Xs&&>(xs)));
            }

            template <typename Pred, typename Xs>
            static constexpr decltype(auto)
            helper(Pred&& pred, Xs&& xs, detail::std::false_type) {
                return static_cast<Pred&&>(pred)(
                        hana::head(static_cast<Xs&&>(xs)));
            }

            template <typename Pred, typename Xs>
            static constexpr decltype(auto)
            helper(Pred&& pred, Xs&& xs, detail::std::true_type)
            { return false_; }

            template <typename Pred, typename Xs>
            constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
                return helper(
                    static_cast<Pred&&>(pred),
                    static_cast<Xs&&>(xs),
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
                static_cast<Xs&&>(xs),
                hana::partial(sequence_detail::take_while_predicate{},
                              static_cast<Pred&&>(pred)),
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
                    unfoldl_impl::apply(static_cast<F&&>(f),
                                  hana::first(static_cast<P&&>(p))),
                    hana::second(static_cast<P&&>(p))
                );
            }
        };

        template <typename F, typename Init>
        static constexpr decltype(auto) apply(F&& f, Init&& init) {
            return hana::maybe(empty<S>(),
                hana::partial(unfoldl_helper{}, f),
                f(static_cast<Init&&>(init))
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
                    hana::first(static_cast<P&&>(p)),
                    unfoldr_impl::apply(static_cast<F&&>(f),
                        hana::second(static_cast<P&&>(p)))
                );
            }
        };

        template <typename F, typename Init>
        static constexpr decltype(auto) apply(F&& f, Init&& init) {
            return hana::maybe(empty<S>(),
                hana::partial(unfoldr_helper{}, f),
                f(static_cast<Init&&>(init))
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
        { return hana::unpack(static_cast<Xs&&>(xs), zip); }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.shortest.with
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename F, typename Xs, typename ...Ys>
    constexpr decltype(auto)
    _zip_shortest_with::operator()(F&& f, Xs&& xs, Ys&& ...ys) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        constexpr bool models_of_Sequence[] = {
            _models<Sequence, Xs>{}, _models<Sequence, Ys>{}...
        };
        static_assert(hana::all(models_of_Sequence),
        "hana::zip.shortest.with(f, xs, ys...) requires xs and ys... to be Sequences");
#endif
        return zip_shortest_with_impl<typename datatype<Xs>::type>::apply(
            static_cast<F&&>(f),
            static_cast<Xs&&>(xs),
            static_cast<Ys&&>(ys)...
        );
    }
    //! @endcond

    template <typename S, typename>
    struct zip_shortest_with_impl : zip_shortest_with_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_shortest_with_impl<S, when<condition>> : default_ {
        template <typename F, typename ...Xs>
        static constexpr decltype(auto) apply(F&& f, Xs&& ...xs) {
            auto min = hana::minimum(hana::make<Tuple>(hana::length(xs)...));
            return zip.unsafe.with(static_cast<F&&>(f),
                hana::take(min, static_cast<Xs&&>(xs))...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.shortest
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Xs, typename ...Ys>
    constexpr decltype(auto) _zip_shortest::operator()(Xs&& xs, Ys&& ...ys) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        constexpr bool models_of_Sequence[] = {
            _models<Sequence, Xs>{}, _models<Sequence, Ys>{}...
        };
        static_assert(hana::all(models_of_Sequence),
        "hana::zip.shortest(xs, ys...) requires xs and ys... to be Sequences");
#endif
        return zip_shortest_impl<typename datatype<Xs>::type>::apply(
            static_cast<Xs&&>(xs),
            static_cast<Ys&&>(ys)...
        );
    }
    //! @endcond

    template <typename S, typename>
    struct zip_shortest_impl : zip_shortest_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_shortest_impl<S, when<condition>> : default_ {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return zip.shortest.with(make<Tuple>, static_cast<Xs&&>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.unsafe.with
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename F, typename Xs, typename ...Ys>
    constexpr decltype(auto)
    _zip_unsafe_with::operator()(F&& f, Xs&& xs, Ys&& ...ys) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        constexpr bool models_of_Sequence[] = {
            _models<Sequence, Xs>{}, _models<Sequence, Ys>{}...
        };
        static_assert(hana::all(models_of_Sequence),
        "hana::zip.unsafe.with(f, xs, ys...) requires xs and ys... to be Sequences");
#endif
        return zip_unsafe_with_impl<typename datatype<Xs>::type>::apply(
            static_cast<F&&>(f),
            static_cast<Xs&&>(xs),
            static_cast<Ys&&>(ys)...
        );
    }
    //! @endcond

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
                hana::lazy(empty<S>()),
                hana::lazy(sequence_detail::zip_unsafe_with_helper{})(
                    static_cast<F&&>(f),
                    static_cast<Xs&&>(xs),
                    static_cast<Ys&&>(ys)...
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zip.unsafe
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Xs, typename ...Ys>
    constexpr decltype(auto) _zip_unsafe::operator()(Xs&& xs, Ys&& ...ys) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        constexpr bool models_of_Sequence[] = {
            _models<Sequence, Xs>{}, _models<Sequence, Ys>{}...
        };
        static_assert(hana::all(models_of_Sequence),
        "hana::zip.unsafe(xs, ys...) requires xs and ys... to be Sequences");
#endif
        return zip_unsafe_impl<typename datatype<Xs>::type>::apply(
            static_cast<Xs&&>(xs),
            static_cast<Ys&&>(ys)...
        );
    }
    //! @endcond

    template <typename S, typename>
    struct zip_unsafe_impl : zip_unsafe_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct zip_unsafe_impl<S, when<condition>> : default_ {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return zip.unsafe.with(make<Tuple>, static_cast<Xs&&>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct make_impl<S, when<_models<Sequence, S>{}>> {
        template <typename ...X>
        static constexpr decltype(auto) apply(X&& ...x) {
            return detail::variadic::foldr(prepend, empty<S>(),
                    static_cast<X&&>(x)...
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
    struct equal_impl<T, U, when<_models<Sequence, T>{} && _models<Sequence, U>{}>>
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
    struct less_impl<T, U, when<_models<Sequence, T>{} && _models<Sequence, U>{}>>
        : Sequence::less_impl<T, U>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Functor
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::transform_impl {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::foldr(static_cast<Xs&&>(xs), empty<S>(),
                        hana::compose(prepend, static_cast<F&&>(f)));
        }
    };

    template <typename S>
    struct transform_impl<S, when<_models<Sequence, S>{}>>
        : Sequence::transform_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Applicative
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::lift_impl {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::prepend(static_cast<X&&>(x), empty<S>()); }
    };

    template <typename S>
    struct ap_impl<S, when<_models<Sequence, S>{}>>
        : Monad::ap_impl<S>
    { };

    template <typename S>
    struct lift_impl<S, when<_models<Sequence, S>{}>>
        : Sequence::lift_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Monad
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::flatten_impl {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::foldr(static_cast<Xs&&>(xs), empty<S>(), concat); }
    };

    template <typename S>
    struct flatten_impl<S, when<_models<Sequence, S>{}>>
        : Sequence::flatten_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Partial model of MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::concat_impl {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::foldr(static_cast<Xs&&>(xs),
                               static_cast<Ys&&>(ys), prepend);
        }
    };

    template <typename S>
    struct concat_impl<S, when<_models<Sequence, S>{}>>
        : Sequence::concat_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct foldl_impl<S, when<_models<Sequence, S>{}>>
        : Iterable::foldl_impl<S>
    { };

    template <typename S>
    struct foldr_impl<S, when<_models<Sequence, S>{}>>
        : Iterable::foldr_impl<S>
    { };

    template <typename S>
    struct foldl1_impl<S, when<_models<Sequence, S>{}>>
        : Iterable::foldl1_impl<S>
    { };

    template <typename S>
    struct foldr1_impl<S, when<_models<Sequence, S>{}>>
        : Iterable::foldr1_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct find_if_impl<S, when<_models<Sequence, S>{}>>
        : Iterable::find_if_impl<S>
    { };

    template <typename S>
    struct any_of_impl<S, when<_models<Sequence, S>{}>>
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
                        static_cast<F&&>(f)(static_cast<X&&>(x)),
                        curry<2>(prepend)
                    ),
                    static_cast<Ys&&>(ys)
                );
            }
        };
    }

    template <typename S>
    struct Sequence::traverse_impl {
        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::foldr(static_cast<Xs&&>(xs), lift<A>(empty<S>()),
                hana::partial(sequence_detail::traverse_helper{},
                              static_cast<F&&>(f)));
        }
    };

    template <typename S>
    struct traverse_impl<S, when<_models<Sequence, S>{}>>
        : Sequence::traverse_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic Foldable -> Sequence conversion
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename F>
    struct to_impl<S, F, when<_models<Sequence, S>{}() && _models<Foldable, F>{}()>>
        : embedding<_models<Sequence, F>{}>
    {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::foldr(static_cast<Xs&&>(xs), empty<S>(), prepend); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEQUENCE_HPP
