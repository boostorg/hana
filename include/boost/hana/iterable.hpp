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
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
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

    template <typename It>
    struct default_<at_impl<It>> {
        template <typename Index, typename Xs>
        static constexpr auto apply(Index n, Xs xs) {
            using I = typename datatype<Index>::type;
            return eval_if(equal(n, zero<I>()),
                [=](auto _) { return _(head)(xs); },
                [=](auto _) { return apply(_(pred)(n), _(tail)(xs)); }
            );
        }
    };

    template <typename It>
    struct default_<last_impl<It>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return eval_if(is_empty(tail(xs)),
                [=](auto _) { return _(head)(xs); },
                [=](auto _) { return apply(_(tail)(xs)); }
            );
        }
    };

    template <typename It>
    struct default_<drop_impl<It>> {
        template <typename N, typename Xs>
        static constexpr auto apply(N n, Xs xs) {
            using I = typename datatype<N>::type;
            return eval_if(or_(equal(n, zero<I>()), is_empty(xs)),
                always(xs),
                [=](auto _) { return apply(_(pred)(n), _(tail)(xs)); }
            );
        }
    };

    template <typename It>
    struct default_<drop_while_impl<It>> {
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

    template <typename It>
    struct default_<drop_until_impl<It>> {
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
    template <typename It>
    struct foldl_impl<It, when<is_an<Iterable, It>{}>> {
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

    template <typename It>
    struct foldr_impl<It, when<is_an<Iterable, It>{}>> {
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

    template <typename It>
    struct foldr1_impl<It, when<is_an<Iterable, It>{}>> {
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

    template <typename It>
    struct foldl1_impl<It, when<is_an<Iterable, It>{}>> {
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
    template <typename It>
    struct find_impl<It, when<is_an<Iterable, It>{}>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            auto e = drop_until(xs, pred);
            return eval_if(is_empty(e),
                always(nothing),
                [=](auto _) { return just(_(head)(e)); }
            );
        }
    };

    template <typename It>
    struct any_impl<It, when<is_an<Iterable, It>{}>> {
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
