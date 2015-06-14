/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_HPP

#include <boost/hana/fwd/iterable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/iterate.hpp>
#include <boost/hana/functional/lockstep.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/searchable.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename Derived>
        struct Iterable_ops {
            template <typename I>
            constexpr decltype(auto) operator[](I&& i) const& {
                return hana::at(
                    static_cast<Derived const&>(*this),
                    static_cast<I&&>(i)
                );
            }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
            template <typename I>
            constexpr decltype(auto) operator[](I&& i) & {
                return hana::at(
                    static_cast<Derived&>(*this),
                    static_cast<I&&>(i)
                );
            }
#endif

            template <typename I>
            constexpr decltype(auto) operator[](I&& i) && {
                return hana::at(
                    static_cast<Derived&&>(*this),
                    static_cast<I&&>(i)
                );
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // head
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct head_impl : head_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct head_impl<It, when<condition>> : default_ {
        static void apply(...);
    };

    //////////////////////////////////////////////////////////////////////////
    // tail
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct tail_impl : tail_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct tail_impl<It, when<condition>> : default_ {
        static void apply(...);
    };

    //////////////////////////////////////////////////////////////////////////
    // is_empty
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct is_empty_impl : is_empty_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct is_empty_impl<It, when<condition>> : default_ {
        static void apply(...);
    };

    //////////////////////////////////////////////////////////////////////////
    // at
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct at_impl : at_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct at_impl<It, when<condition>> : default_ {
        struct next {
            template <typename Xs, typename Index>
            constexpr decltype(auto) operator()(Xs&& xs, Index&& n) const {
                return at_impl::apply(hana::tail(static_cast<Xs&&>(xs)),
                                      hana::pred(n));
            }
        };

        template <typename Xs, typename Index>
        static constexpr decltype(auto) apply(Xs&& xs, Index&& n) {
            using I = typename datatype<Index>::type;
            return hana::eval_if(hana::equal(n, zero<I>()),
                hana::lazy(head)(xs),
                hana::lazy(next{})(xs, n)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // at_c
    //////////////////////////////////////////////////////////////////////////
    template <std::size_t n>
    struct _at_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return hana::at(static_cast<Xs&&>(xs), size_t<n>); }
    };

    //////////////////////////////////////////////////////////////////////////
    // last
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct last_impl : last_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct last_impl<It, when<condition>> : default_ {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return hana::eval_if(hana::is_empty(hana::tail(xs)),
                hana::always(hana::head(xs)),
                hana::lazy(hana::compose(last, tail))(xs)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop.at_most
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct drop_at_most_impl : drop_at_most_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct drop_at_most_impl<It, when<condition>> : default_ {
        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N&& n) {
            using I = typename datatype<N>::type;
            return hana::eval_if(
                hana::or_(hana::equal(n, zero<I>()), hana::is_empty(xs)),
                hana::always(xs),
                hana::lazy(hana::lockstep(drop.at_most)(tail, pred))(xs, n)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop.exactly
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct drop_exactly_impl : drop_exactly_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct drop_exactly_impl<It, when<condition>> : default_ {
        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr auto n = hana::value<N>();
            return hana::iterate<n>(hana::tail)(static_cast<Xs&&>(xs));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop_c
    //////////////////////////////////////////////////////////////////////////
    template <std::size_t n>
    struct _drop_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return hana::drop(static_cast<Xs&&>(xs), size_t<n>); }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop_while
    //////////////////////////////////////////////////////////////////////////
    namespace iterable_detail {
        struct drop_while_helper {
            struct next {
                template <typename Xs, typename Pred>
                constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                    return hana::drop_while(
                        hana::tail(static_cast<Xs&&>(xs)),
                        static_cast<Pred&&>(pred)
                    );
                }
            };

            template <typename Xs, typename Pred>
            constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                return hana::eval_if(pred(hana::head(xs)),
                    hana::lazy(next{})(xs, pred),
                    hana::lazy(xs)
                );
            }
        };
    }

    template <typename It, typename>
    struct drop_while_impl : drop_while_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct drop_while_impl<It, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(xs),
                hana::lazy(iterable_detail::drop_while_helper{})(
                                            xs, static_cast<Pred&&>(pred))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop_until
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct drop_until_impl : drop_until_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct drop_until_impl<It, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::drop_while(
                static_cast<Xs&&>(xs),
                hana::compose(not_, static_cast<Pred&&>(pred))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename It>
    struct models_impl<Iterable, It>
        : _integral_constant<bool,
            !is_default<head_impl<It>>{}() &&
            !is_default<tail_impl<It>>{}() &&
            !is_default<is_empty_impl<It>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model of Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename It>
    struct Iterable::fold_left_impl {
        template <typename Xs, typename State, typename F>
        static constexpr auto
        fold_left_helper(Xs&&, State&& s, F&&, decltype(true_)) {
            return static_cast<State&&>(s);
        }

        template <typename Xs, typename State, typename F>
        static constexpr decltype(auto)
        fold_left_helper(Xs&& xs, State&& s, F&& f, decltype(false_)) {
            constexpr bool done = hana::value<decltype(
                hana::is_empty(hana::tail(xs))
            )>();
            return fold_left_impl::fold_left_helper(
                hana::tail(xs), f(s, hana::head(xs)), f, hana::bool_<done>
            );
        }

        template <typename Xs, typename State, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, State&& s, F&& f) {
            constexpr bool done = hana::value<decltype(hana::is_empty(xs))>();
            return fold_left_impl::fold_left_helper(
                static_cast<Xs&&>(xs),
                static_cast<State&&>(s),
                static_cast<F&&>(f),
                hana::bool_<done>
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return fold_left_impl::apply(hana::tail(xs),
                                         hana::head(xs),
                                         static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename It>
    struct Iterable::find_if_impl {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::only_when(hana::compose(not_, is_empty), head,
                hana::drop_until(static_cast<Xs&&>(xs),
                                 static_cast<Pred&&>(pred))
            );
        }
    };

    template <typename It>
    struct Iterable::any_of_impl {
        template <bool Done, typename Dummy = void>
        struct any_of_helper {
            template <typename Xs, typename Pred>
            static constexpr auto apply(bool prev_cond, Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::head(xs)), true_, false_);
                decltype(auto) tail = hana::tail(static_cast<Xs&&>(xs));
                constexpr bool done = hana::value<decltype(hana::is_empty(tail))>();
                return prev_cond ? true_
                    : any_of_impl::any_of_helper<done>::apply(cond,
                                static_cast<decltype(tail)&&>(tail),
                                static_cast<Pred&&>(pred));
            }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(true_), Xs&&, Pred&&)
            { return true_; }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(false_), Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::head(xs)), true_, false_);
                constexpr bool done = hana::value<decltype(
                    hana::is_empty(hana::tail(xs))
                )>();
                return any_of_impl::any_of_helper<done>::apply(cond,
                                        hana::tail(static_cast<Xs&&>(xs)),
                                        static_cast<Pred&&>(pred));
            }
        };

        template <typename Dummy>
        struct any_of_helper<true, Dummy> {
            template <typename Cond, typename Xs, typename Pred>
            static constexpr auto apply(Cond cond, Xs&&, Pred&&)
            { return cond; }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr bool done = hana::value<decltype(hana::is_empty(xs))>();
            return any_of_impl::any_of_helper<done>::apply(false_,
                                            static_cast<Xs&&>(xs),
                                            static_cast<Pred&&>(pred));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
