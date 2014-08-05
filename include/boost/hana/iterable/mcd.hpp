/*!
@file
Defines `boost::hana::Iterable::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_MCD_HPP
#define BOOST_HANA_ITERABLE_MCD_HPP

#include <boost/hana/iterable/iterable.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `head`, `tail` and `is_empty`
    struct Iterable::mcd {
        template <typename Index, typename Iterable_>
        static constexpr auto at_impl(Index n, Iterable_ iterable) {
            return eval_if(equal(n, size_t<0>),
                [=](auto _) { return head(_(iterable)); },
                [=](auto _) { return at_impl(_(n) - size_t<1>, tail(_(iterable))); }
            );
        }

        template <typename Iterable_>
        static constexpr auto last_impl(Iterable_ iterable) {
            return eval_if(is_empty(tail(iterable)),
                [=](auto _) { return head(_(iterable)); },
                [=](auto _) { return last_impl(tail(_(iterable))); }
            );
        }

        template <typename N, typename Iterable_>
        static constexpr auto drop_impl(N n, Iterable_ iterable) {
            return eval_if(or_(equal(n, size_t<0>), is_empty(iterable)),
                [=](auto) { return iterable; },
                [=](auto _) { return drop_impl(_(n) - size_t<1>, tail(_(iterable))); }
            );
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto drop_while_impl(Pred pred, Iterable_ iterable) {
            return eval_if(is_empty(iterable),
                [=](auto) { return iterable; },
                [=](auto _) {
                    return eval_if(pred(_(head)(iterable)),
                        [=](auto _) { return drop_while_impl(pred, _(tail)(iterable)); },
                        [=](auto) { return iterable; }
                    );
                }
            );
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto drop_until_impl(Pred pred, Iterable_ iterable) {
            return drop_while([=](auto x) { return not_(pred(x)); }, iterable);
        }

        template <typename It, typename F>
        static constexpr auto for_each_impl(It it, F f) {
            return eval_if(is_empty(it),
                [](auto) { },
                [=](auto _) {
                    f(_(head)(it));
                    for_each_impl(_(tail)(it), f);
                }
            );
        }
    };

    namespace detail {
        struct FoldableFromIterable : Foldable::mcd {
            template <typename Xs, typename State, typename F>
            static constexpr auto foldl_impl(Xs xs, State s, F f) {
                return eval_if(is_empty(xs),
                    [=](auto) { return s; },
                    [=](auto _) {
                        return foldl_impl(tail(_(xs)), f(s, head(_(xs))), f);
                    }
                );
            }

            template <typename Xs, typename F>
            static constexpr auto foldl1_impl(Xs xs, F f)
            { return foldl(tail(xs), head(xs), f); }

            template <typename Xs, typename F>
            static constexpr auto foldr1_impl(Xs xs, F f) {
                return eval_if(is_empty(tail(xs)),
                    [=](auto) { return head(xs); },
                    [=](auto _) {
                        return f(head(xs), foldr1_impl(tail(_(xs)), f));
                    }
                );
            }

            template <typename Xs, typename State, typename F>
            static constexpr auto foldr_impl(Xs xs, State s, F f) {
                return eval_if(is_empty(xs),
                    [=](auto) { return s; },
                    [=](auto _) {
                        return f(head(_(xs)), foldr_impl(tail(_(xs)), s, f));
                    }
                );
            }
        };
    }

    //! Every `Iterable` instance gives rise to an instance of `Foldable`,
    //! which is valid for all finite such `Iterable`s.
    //!
    //! @details
    //! Let `xs` be an `Iterable` and let `xi` denote its `i`-th element. In
    //! other words, `xs` can be folded into a list containing `[x1, ..., xN]`,
    //! where `N` is the number of elements. Right-folding `xs` with a binary
    //! operation `*` (in infix notation for legibility) is equivalent to
    //! @code
    //!     x1 * (x2 * ( ... * (xN-1 * xN)))
    //! @endcode
    //!
    //! Similarly, left-folding `xs` is equivalent to
    //! @code
    //!     (((x1 * x2) * x3) * ...) * xN
    //! @endcode
    //!
    //! In both cases, notice the side of the parentheses. Left-folding
    //! applies `*` in a left-associative manner, whereas right-folding
    //! applies it in a right-associative manner. For associative operations,
    //! i.e. operations such that for all `a`, `b` and `c`,
    //! @code
    //!     (a * b) * c = a * (b * c)
    //! @endcode
    //! this makes no difference. Also note that lazy folds and folds with an
    //! initial state are implemented in an analogous way.
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/foldl.cpp main
    //!
    //! ### Example 2
    //! @snippet example/integer_list/foldable/foldr.cpp main
    template <typename T>
    struct Foldable::instance<T, when<is_an<Iterable, T>()>>
        : detail::FoldableFromIterable
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_MCD_HPP
