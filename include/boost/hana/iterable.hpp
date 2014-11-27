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

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
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

    //! Minimal complete definition: `head`, `tail` and `is_empty`
    struct Iterable::mcd {
        template <typename Index, typename Iterable_>
        static constexpr auto at_impl(Index n, Iterable_ iterable) {
            using I = datatype_t<Index>;
            return eval_if(equal(n, zero<I>()),
                [=](auto _) { return _(head)(iterable); },
                [=](auto _) { return at_impl(_(pred)(n), _(tail)(iterable)); }
            );
        }

        template <typename Iterable_>
        static constexpr auto last_impl(Iterable_ iterable) {
            return eval_if(is_empty(tail(iterable)),
                [=](auto _) { return _(head)(iterable); },
                [=](auto _) { return last_impl(_(tail)(iterable)); }
            );
        }

        template <typename N, typename Iterable_>
        static constexpr auto drop_impl(N n, Iterable_ iterable) {
            using I = datatype_t<N>;
            return eval_if(or_(equal(n, zero<I>()), is_empty(iterable)),
                always(iterable),
                [=](auto _) { return drop_impl(_(pred)(n), _(tail)(iterable)); }
            );
        }

        template <typename Xs, typename Pred>
        static constexpr auto drop_while_impl(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                always(xs),
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) { return drop_while_impl(_(tail)(xs), pred); },
                        [=](auto) { return xs; }
                    );
                }
            );
        }

        template <typename Xs, typename Pred>
        static constexpr decltype(auto) drop_until_impl(Xs&& xs, Pred&& pred) {
            return drop_while(
                detail::std::forward<Xs>(xs),
                compose(not_, detail::std::forward<Pred>(pred))
            );
        }
    };

    //! Minimal complete definition: `Iterable`
    //!
    //! @todo Add perfect forwarding where possible. This is _not_ obvious
    //! to me because of all those branches.
    struct Foldable::iterable_mcd : Foldable::folds_mcd {
        template <typename Xs, typename State, typename F>
        static constexpr auto foldl_impl(Xs xs, State s, F f) {
            return eval_if(is_empty(xs),
                always(s),
                [xs, s, f](auto _) {
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
                [xs](auto) { return head(xs); },
                [xs, f](auto _) {
                    return f(head(xs), foldr1_impl(_(tail)(xs), f));
                }
            );
        }

        template <typename Xs, typename State, typename F>
        static constexpr auto foldr_impl(Xs xs, State s, F f) {
            return eval_if(is_empty(xs),
                always(s),
                [xs, s, f](auto _) {
                    return f(_(head)(xs), foldr_impl(_(tail)(xs), s, f));
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

    //! Minimal complete definition: `Iterable`
    //!
    //! @todo
    //! Use perfect forwarding once bug
    //! http://llvm.org/bugs/show_bug.cgi?id=20619
    //! is fixed.
    struct Searchable::iterable_mcd : Searchable::mcd {
        template <typename Xs, typename Pred>
        static constexpr auto find_impl(Xs xs, Pred pred) {
            auto e = drop_until(xs, pred);
            return eval_if(is_empty(e),
                always(nothing),
                [=](auto _) { return just(_(head)(e)); }
            );
        }

        template <typename Xs, typename Pred>
        static constexpr auto any_impl(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                always(false_),
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        always(true_),
                        [=](auto _) { return any_impl(_(tail)(xs), pred); }
                    );
                }
            );
        }
    };

    //! Instance of `Searchable` for `Iterable`s.
    //!
    //! An `Iterable` can be searched by doing a linear search in the elements,
    //! with the keys and values both being the elements in the iterable.
    //!
    //! ### Example
    //! @snippet example/iterable.cpp find
    template <typename T>
    struct Searchable::instance<T, when<is_an<Iterable, T>()>>
        : Searchable::iterable_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
