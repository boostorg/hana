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
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/searchable.hpp>


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
                    detail::std::forward<I>(i),
                    static_cast<Derived const&>(*this)
                );
            }

            template <typename I>
            constexpr decltype(auto) operator[](I&& i) & {
                return hana::at(
                    detail::std::forward<I>(i),
                    static_cast<Derived&>(*this)
                );
            }

            template <typename I>
            constexpr decltype(auto) operator[](I&& i) && {
                return hana::at(
                    detail::std::forward<I>(i),
                    static_cast<Derived&&>(*this)
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
        template <typename Xs>
        static constexpr void apply(Xs&&) {
            static_assert(wrong<head_impl<It>, Xs>{},
            "no definition of boost::hana::head for the given data types");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // tail
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct tail_impl : tail_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct tail_impl<It, when<condition>> : default_ {
        template <typename Xs>
        static constexpr void apply(Xs&&) {
            static_assert(wrong<tail_impl<It>, Xs>{},
            "no definition of boost::hana::tail for the given data types");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // is_empty
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct is_empty_impl : is_empty_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct is_empty_impl<It, when<condition>> {
        template <typename Xs>
        static constexpr void apply(Xs&&) {
            static_assert(wrong<is_empty_impl<It>, Xs>{},
            "no definition of boost::hana::is_empty for the given data types");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // at
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct at_impl : at_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct at_impl<It, when<condition>> : default_ {
        template <typename Index, typename Xs>
        static constexpr auto apply(Index n, Xs xs) {
            using I = typename datatype<Index>::type;
            return hana::eval_if(hana::equal(n, zero<I>()),
                [=](auto _) { return _(head)(xs); },
                [=](auto _) { return apply(_(pred)(n), _(tail)(xs)); }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // at_c
    //////////////////////////////////////////////////////////////////////////
    template <detail::std::size_t n>
    struct _at_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return hana::at(size_t<n>, detail::std::forward<Xs>(xs)); }
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
                [=](auto _) { return _(head)(xs); },
                [=](auto _) { return apply(_(tail)(xs)); }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct drop_impl : drop_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct drop_impl<It, when<condition>> : default_ {
        template <typename N, typename Xs>
        static constexpr auto apply(N n, Xs xs) {
            using I = typename datatype<N>::type;
            return hana::eval_if(
                hana::or_(hana::equal(n, zero<I>()), hana::is_empty(xs)),
                hana::always(xs),
                [=](auto _) { return apply(_(pred)(n), _(tail)(xs)); }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop_c
    //////////////////////////////////////////////////////////////////////////
    template <detail::std::size_t n>
    struct _drop_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return hana::drop(size_t<n>, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // drop_while
    //////////////////////////////////////////////////////////////////////////
    template <typename It, typename>
    struct drop_while_impl : drop_while_impl<It, when<true>> { };

    template <typename It, bool condition>
    struct drop_while_impl<It, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::always(xs),
                [=](auto _) {
                    return hana::eval_if(pred(_(head)(xs)),
                        [=](auto _) { return apply(_(tail)(xs), pred); },
                        [=](auto) { return xs; }
                    );
                }
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
                detail::std::forward<Xs>(xs),
                hana::compose(not_, detail::std::forward<Pred>(pred))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename It>
    struct models<Iterable(It)>
        : detail::std::integral_constant<bool,
            !is_default<head_impl<It>>{} &&
            !is_default<tail_impl<It>>{} &&
            !is_default<is_empty_impl<It>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model of Foldable
    //////////////////////////////////////////////////////////////////////////
    namespace iterable_detail {
        struct foldl_helper {
            template <typename F, typename X>
            constexpr decltype(auto) operator()(F&& f, X&& x) const {
                decltype(auto) state = hana::first(detail::std::forward<X>(x));
                decltype(auto) xs = hana::second(detail::std::forward<X>(x));
                return hana::pair(
                    detail::std::forward<F>(f)(
                        detail::std::forward<decltype(state)>(state),
                        hana::head(xs)
                    ),
                    hana::tail(xs)
                );
            }
        };
    }

    template <typename It>
    struct Iterable::foldl_impl {
        template <typename Xs, typename State, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, State&& s, F&& f) {
            return hana::first(hana::until(
                hana::compose(is_empty, second),
                hana::pair(detail::std::forward<State>(s),
                           detail::std::forward<Xs>(xs)),
                hana::partial(iterable_detail::foldl_helper{},
                              detail::std::forward<F>(f))
            ));
        }
    };

    template <typename It>
    struct Iterable::foldr_impl {
        template <typename Xs, typename State, typename F>
        static constexpr auto apply(Xs xs, State s, F f) {
            return hana::eval_if(hana::is_empty(xs),
                hana::always(s),
                [xs, s, f](auto _) {
                    return f(_(head)(xs), apply(_(tail)(xs), s, f));
                }
            );
        }
    };

    template <typename It>
    struct Iterable::foldr1_impl {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return hana::eval_if(hana::is_empty(hana::tail(xs)),
                [xs](auto) { return hana::head(xs); },
                [xs, f](auto _) {
                    return f(hana::head(xs), apply(_(tail)(xs), f));
                }
            );
        }
    };

    template <typename It>
    struct Iterable::foldl1_impl {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::foldl(hana::tail(xs), hana::head(xs),
                        detail::std::forward<F>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename It>
    struct Iterable::find_impl {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            auto e = hana::drop_until(xs, pred);
            return hana::eval_if(hana::is_empty(e),
                hana::always(nothing),
                [=](auto _) { return hana::just(_(head)(e)); }
            );
        }
    };

    template <typename It>
    struct Iterable::any_of_impl {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs xs, Pred pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::always(false_),
                [=](auto _) {
                    return hana::eval_if(pred(_(head)(xs)),
                        hana::always(true_),
                        [=](auto _) { return apply(_(tail)(xs), pred); }
                    );
                }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
