/*!
@file
Defines `boost::hana::Iterable::FoldableInstance`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_FOLDABLE_INSTANCE_HPP
#define BOOST_HANA_ITERABLE_FOLDABLE_INSTANCE_HPP

#include <boost/hana/iterable/iterable.hpp>

#include <boost/hana/core.hpp> // for instantiates
#include <boost/hana/foldable/lazy_foldr_mcd.hpp>
#include <boost/hana/functional.hpp> // for partial
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>


namespace boost { namespace hana {
    struct Iterable::FoldableInstance : Foldable::lazy_foldr_mcd {
        template <typename F, typename State, typename Iterable>
        static constexpr auto foldl_impl(F f, State s, Iterable xs) {
            return eval_if(is_empty(xs),
                [=](auto) { return s; },
                [=](auto _) { return foldl_impl(f, f(s, head(_(xs))), tail(_(xs))); }
            );
        }

        template <typename F, typename Iterable>
        static constexpr auto foldl1_impl(F f, Iterable xs)
        { return foldl(f, head(xs), tail(xs)); }

        template <typename F, typename Iterable>
        static constexpr auto foldr1_impl(F f, Iterable xs) {
            return eval_if(is_empty(tail(xs)),
                [=](auto) { return head(xs); },
                [=](auto _) { return f(head(xs), foldr1_impl(f, tail(_(xs)))); }
            );
        }

        template <typename F, typename State, typename Iterable>
        static constexpr auto foldr_impl(F f, State s, Iterable xs) {
            return eval_if(is_empty(xs),
                [=](auto) { return s; },
                [=](auto _) { return f(head(_(xs)), foldr_impl(f, s, tail(_(xs)))); }
            );
        }

        template <typename F, typename State, typename Iterable>
        static constexpr auto lazy_foldr_impl(F f, State s, Iterable xs) {
            return eval_if(is_empty(xs),
                [=](auto) { return s; },
                [=](auto _) {
                    return f(partial(head, xs), partial(lazy_foldr, f, s, tail(_(xs))));
                }
            );
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto find_impl(Pred pred, Iterable_ xs) {
            auto e = drop_until(pred, xs);
            return eval_if(is_empty(e),
                [](auto) { return nothing; },
                [=](auto _) { return just(_(head)(e)); }
            );
        }
    };

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
    //! @f{align*}{
    //!     (a * b) * c = a * (b * c)
    //! @f}
    //! this makes no difference. Also note that lazy folds and folds with an
    //! initial state are implemented in an analogous way.
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/foldl.cpp main
    //!
    //! ### Example 2
    //! @snippet example/integer_list/foldable/foldr.cpp main
    template <typename T>
    struct Foldable::instance<T, when<instantiates<Iterable, T>()>>
        : Iterable::FoldableInstance
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_FOLDABLE_INSTANCE_HPP
