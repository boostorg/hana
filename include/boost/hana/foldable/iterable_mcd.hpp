/*!
@file
Defines `boost::hana::Foldable::iterable_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_ITERABLE_MCD_HPP
#define BOOST_HANA_FOLDABLE_ITERABLE_MCD_HPP

#include <boost/hana/foldable/folds_mcd.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! @details
    //! Minimal complete definition: `Iterable`
    //!
    //! @todo Add perfect forwarding where possible. This is _not_ obvious
    //! to me because of all those branches.
    struct Foldable::iterable_mcd : Foldable::folds_mcd {
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
    //! @snippet example/list/foldable.cpp foldl
    //!
    //! ### Example 2
    //! @snippet example/integer_list/foldable.cpp foldr
    template <typename T>
    struct Foldable::instance<T, when<is_an<Iterable, T>()>>
        : Foldable::iterable_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_ITERABLE_MCD_HPP
