/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_HPP

#include <boost/hana/fwd/iterable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monoid.hpp>

// provided instances
#include <boost/hana/foldable.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Inherit this to get an `operator[]` equivalent to `at`.
        //! @relates boost::hana::Iterable
        //!
        //! @todo Find a proper way to provide member operators.
        template <typename Derived>
        struct Iterable_ops {
            template <typename I>
            constexpr decltype(auto) operator[](I&& i) const& {
                return at(
                    detail::std::forward<I>(i),
                    static_cast<Derived const&>(*this)
                );
            }

            template <typename I>
            constexpr decltype(auto) operator[](I&& i) & {
                return at(
                    detail::std::forward<I>(i),
                    static_cast<Derived&>(*this)
                );
            }

            template <typename I>
            constexpr decltype(auto) operator[](I&& i) const&& {
                return at(
                    detail::std::forward<I>(i),
                    static_cast<Derived const&&>(*this)
                );
            }

            template <typename I>
            constexpr decltype(auto) operator[](I&& i) && {
                return at(
                    detail::std::forward<I>(i),
                    static_cast<Derived&&>(*this)
                );
            }
        };
    }

    template <typename Xs, typename Context>
    struct at_impl<Xs, when<
        is_implemented<head_impl<Xs>, Context> &&
        is_implemented<tail_impl<Xs>, Context>
    >, Context> {
        template <typename Index, typename Iterable_>
        static constexpr auto apply(Index n, Iterable_ iterable) {
            using I = datatype_t<Index>;
            return eval_if(equal(n, zero<I>()),
                [=](auto _) { return _(head)(iterable); },
                [=](auto _) { return apply(_(pred)(n), _(tail)(iterable)); }
            );
        }
    };

    template <typename Xs, typename Context>
    struct last_impl<Xs, when<
        is_implemented<head_impl<Xs>, Context> &&
        is_implemented<tail_impl<Xs>, Context> &&
        is_implemented<is_empty_impl<Xs>, Context>
    >, Context> {
        template <typename Iterable_>
        static constexpr auto apply(Iterable_ iterable) {
            return eval_if(is_empty(tail(iterable)),
                [=](auto _) { return _(head)(iterable); },
                [=](auto _) { return apply(_(tail)(iterable)); }
            );
        }
    };

    template <typename Xs, typename Context>
    struct drop_impl<Xs, when<
        is_implemented<tail_impl<Xs>, Context> &&
        is_implemented<is_empty_impl<Xs>, Context>
    >, Context> {
        template <typename N, typename Iterable_>
        static constexpr auto apply(N n, Iterable_ iterable) {
            using I = datatype_t<N>;
            return eval_if(or_(equal(n, zero<I>()), is_empty(iterable)),
                always(iterable),
                [=](auto _) { return apply(_(pred)(n), _(tail)(iterable)); }
            );
        }
    };

    template <typename It, typename Context>
    struct drop_while_impl<It, when<
        is_implemented<head_impl<It>, Context> &&
        is_implemented<tail_impl<It>, Context> &&
        is_implemented<is_empty_impl<It>, Context>
    >, Context> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                always(xs),
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) { return apply(_(tail)(xs), pred); },
                        [=](auto) { return xs; }
                    );
                }
            );
        }
    };

    template <typename It, typename Context>
    struct drop_until_impl<It, when<
        is_implemented<drop_while_impl<It>, Context>
    >, Context> {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return drop_while(
                detail::std::forward<Xs>(xs),
                compose(not_, detail::std::forward<Pred>(pred))
            );
        }
    };

    //! @todo Add perfect forwarding where possible. This is _not_ obvious
    //! to me because of all those branches.
    template <typename T>
    struct foldl_impl<T, when<is_an<Iterable, T>()>> {
        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State s, F f) {
            return eval_if(is_empty(xs),
                always(s),
                [xs, s, f](auto _) {
                    return apply(tail(_(xs)), f(s, head(_(xs))), f);
                }
            );
        }
    };

    template <typename T>
    struct foldr_impl<T, when<is_an<Iterable, T>()>> {
        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State s, F f) {
            return eval_if(is_empty(xs),
                always(s),
                [xs, s, f](auto _) {
                    return f(_(head)(xs), apply(_(tail)(xs), s, f));
                }
            );
        }
    };

    template <typename T>
    struct foldr1_impl<T, when<is_an<Iterable, T>()>> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return eval_if(is_empty(tail(xs)),
                [xs](auto) { return head(xs); },
                [xs, f](auto _) {
                    return f(head(xs), apply(_(tail)(xs), f));
                }
            );
        }
    };

    template <typename T>
    struct foldl1_impl<T, when<is_an<Iterable, T>()>> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f)
        { return foldl(tail(xs), head(xs), f); }
    };

    //! Instance of `Searchable` for `Iterable`s.
    //!
    //! An `Iterable` can be searched by doing a linear search in the elements,
    //! with the keys and values both being the elements in the iterable.
    //!
    //! ### Example
    //! @snippet example/iterable.cpp find
    //!
    //! @todo
    //! Use perfect forwarding once Clang bug #20619 is fixed.
    template <typename T>
    struct find_impl<T, when<is_an<Iterable, T>()>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            auto e = drop_until(xs, pred);
            return eval_if(is_empty(e),
                always(nothing),
                [=](auto _) { return just(_(head)(e)); }
            );
        }
    };

    template <typename T>
    struct any_impl<T, when<is_an<Iterable, T>()>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                always(false_),
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        always(true_),
                        [=](auto _) { return apply(_(tail)(xs), pred); }
                    );
                }
            );
        }
    };

    template <typename It>
    constexpr auto is_a<Iterable, It> = bool_<
        is_implemented<head_impl<It>> &&
        is_implemented<tail_impl<It>> &&
        is_implemented<is_empty_impl<It>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
