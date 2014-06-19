/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    Data structures allowing external iteration.

    --------------------------------------------------------------------------

    ## Minimal complete definition
    `head`, `tail` and `is_empty`

    --------------------------------------------------------------------------

    ## Provided instances

    ### Comparable
    Two `Iterable`s are equal if and only if they contain the same number of
    elements and their elements at any given index are equal. To enable the
    `Comparable` instance for a data type, specialize the
    `comparable_from_iterable` variable template.
    @snippet example/list/comparable.cpp main

    ### Foldable
    Let `xs` be an `Iterable` and let `xi` denote its `i`-th element. In other
    words, `xs` is equivalent to a list containing `[x1, ..., xN]`, where `N`
    is the number of elements. Right-folding `xs` with a binary operation `*`
    (in infix notation for legibility) is equivalent to
    @code
        x1 * (x2 * ( ... * (xN-1 * xN)))
    @endcode

    Similarly, left-folding `xs` is equivalent to
    @code
        (((x1 * x2) * x3) * ...) * xN
    @endcode

    In both cases, notice the side of the parentheses. Left-folding applies
    `*` in a left-associative manner, whereas right-folding applies it in
    a right-associative manner. For associative operations, i.e. operations
    such that for all `a`, `b` and `c`,
    @f{align*}{
        (a * b) * c = a * (b * c)
    @f}
    this makes no difference. Also note that lazy folds and folds with an
    initial state are implemented in an analogous way. To enable the `Foldable`
    instance for a data type, specialize the `foldable_from_iterable` variable
    template.
    #### Example 1
    @snippet example/list/foldable/foldl.cpp fusion
    #### Example 2
    @snippet example/list/foldable/foldr.cpp fusion

    --------------------------------------------------------------------------

    @note
    In the description of the methods, all indexing is 0-based.

    @todo
    - What about infinite `Iterable`s?
    - There are probably tons of laws that must be respected?
    - Instead of having a lot of methods, maybe some of the functions below
      should just be implemented as functions using the mcd, as in the MPL11?
     */
    template <typename T, typename Enable = void>
    struct Iterable;

    //! Return the first element of a non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto head = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::head_impl(iterable);
    };

    //! Return a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto tail = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::tail_impl(iterable);
    };

    //! Return whether the iterable is empty.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_empty = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::is_empty_impl(iterable);
    };

    //! Return the `n`th element of an iterable.
    //! @method{Iterable}
    //!
    //! ### Example
    //! @snippet example/list/iterable/at.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto at = [](auto n, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::at_impl(n, iterable);
    };

    //! Return the last element of a non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto last = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::last_impl(iterable);
    };

    //! Drop the first `n` elements of an iterable and return the rest.
    //! @method{Iterable}
    //!
    //! `n` must be a non-negative `Integral` representing the number of
    //! elements to be dropped from the iterable. If `n` is greater than
    //! the length of the iterable, the returned iterable is empty.
    //!
    //! ### Example
    //! @snippet example/list/iterable/drop.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop = [](auto n, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::drop_impl(n, iterable);
    };

    //! Drop elements from an iterable up to, but not including, the first
    //! element for which the `predicate` is not satisfied.
    //! @method{Iterable}
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //!
    //! ### Example
    //! @snippet example/range/iterable/drop_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_while = [](auto predicate, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::drop_while_impl(predicate, iterable);
    };

    //! Equivalent to `drop_while` with a negated `predicate`.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_until = [](auto predicate, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::drop_until_impl(predicate, iterable);
    };

    template <>
    struct instance<Iterable> {
        template <typename T, typename Enable = void>
        struct with { };
    };

    template <>
    struct defaults<Iterable> {
        template <typename T, typename Enable = void>
        struct with : defaults<> {
            template <typename Index, typename Iterable_>
            static constexpr auto at_impl(Index n, Iterable_ iterable) {
                return eval_if(n == size_t<0>,
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
                return eval_if(n == size_t<0> || is_empty(iterable),
                    always(iterable),
                    [=](auto _) { return drop_impl(_(n) - size_t<1>, tail(_(iterable))); }
                );
            }

            template <typename Pred, typename Iterable_>
            static constexpr auto drop_while_impl(Pred pred, Iterable_ iterable) {
                return if_(is_empty(iterable),
                    always(iterable),
                    [=](auto it) {
                        return if_(pred(head(it)),
                            [=](auto it) { return drop_while_impl(pred, tail(it)); },
                            always(it)
                        )(it);
                    }
                )(iterable);
            }

            template <typename Pred, typename Iterable_>
            static constexpr auto drop_until_impl(Pred pred, Iterable_ iterable) {
                return drop_while([=](auto x) { return !pred(x); }, iterable);
            }
        };
    };

    template <typename T, typename Enable>
    struct Iterable : instance<Iterable>::template with<T> { };

    template <typename T>
    constexpr bool comparable_from_iterable = false;

    template <typename T, typename U>
    struct instance<Comparable>::with<T, U,
        detail::enable_if_t<
            comparable_from_iterable<T> &&
            comparable_from_iterable<U>
        >
    >
        : defaults<Comparable>::template with<T, U>
    {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys) {
            return if_(is_empty(xs) || is_empty(ys),
                [](auto xs, auto ys) {
                    return is_empty(xs) && is_empty(ys);
                },
                [](auto xs, auto ys) {
                    return equal(head(xs), head(ys)) &&
                           equal_impl(tail(xs), tail(ys));
                }
            )(xs, ys);
        }
    };

    template <typename T>
    constexpr bool foldable_from_iterable = false;

    template <typename T>
    struct instance<Foldable>::with<T,
        detail::enable_if_t<foldable_from_iterable<T>>
    >
        : defaults<Foldable>::template with<T>
    {
        template <typename F, typename State, typename Iterable>
        static constexpr auto foldl_impl(F f, State s, Iterable xs) {
            return eval_if(is_empty(xs),
                always(s),
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
                always(s),
                [=](auto _) { return f(head(_(xs)), foldr_impl(f, s, tail(_(xs)))); }
            );
        }

        template <typename F, typename State, typename Iterable>
        static constexpr auto lazy_foldr_impl(F f, State s, Iterable xs) {
            return eval_if(is_empty(xs),
                always(s),
                [=](auto _) {
                    return f(partial(head, xs), partial(lazy_foldr, f, s, tail(_(xs))));
                }
            );
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto find_impl(Pred pred, Iterable_ xs) {
            auto e = drop_until(pred, xs);
            return eval_if(is_empty(e),
                always(nothing),
                [=](auto _) { return just(_(head)(e)); }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
