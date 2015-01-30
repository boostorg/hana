/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ITERABLE_HPP
#define BOOST_HANA_FWD_ITERABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    namespace operators {
        template <typename Derived>
        struct Iterable_ops;
    }

    //! @ingroup group-typeclasses
    //! `Iterable` represents data structures supporting external iteration.
    //!
    //!
    //! Laws
    //! ----
    //! In a sense, `Foldable` represents data structures supporting internal
    //! iteration with the ability to accumulate a result. We request that
    //! instances of both `Iterable` and `Foldable` provide consistent
    //! instances. Hence, for any finite `Iterable` _and_ `Foldable` `it`,
    //! the following laws must be satisfied:
    //! @code
    //!     head(it) == head(to<Tuple>(it))
    //!     to<Tuple>(tail(it)) == tail(to<Tuple>(it))
    //!     is_empty(it) if and only if is_empty(to<Tuple>(it))
    //! @endcode
    //!
    //! This is basically saying that linearizing a `Foldable` and then
    //! iterating through it should be equivalent to iterating through it
    //! in the first place.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `head`, `tail` and `is_empty`
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. `Foldable`\n
    //! Every finite `Iterable` instance gives rise to an instance of
    //! `Foldable`.
    //!
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
    //! 2. `Searchable`\n
    //! An `Iterable` can be searched by doing a linear search in the elements,
    //! with the keys and values both being the elements in the iterable.
    //! @snippet example/iterable.cpp find
    //!
    //!
    //! ### Example 1
    //! @snippet example/list/foldable.cpp foldl
    //!
    //! ### Example 2
    //! @snippet example/integer_list/foldable.cpp foldr
    //!
    //!
    //! @todo
    //! - Either make it a requirement of Iterable to be convertible to a
    //!   Tuple (since it's used in the laws), or require something strictly
    //!   more general.
    //! - Add perfect forwarding in the methods.
    //! - Use perfect forwarding in `Iterable::find_impl` once Clang
    //!   bug #20619 is fixed.
    struct Iterable {
        template <typename It> struct foldl_impl;
        template <typename It> struct foldr_impl;
        template <typename It> struct foldl1_impl;
        template <typename It> struct foldr1_impl;

        template <typename It> struct find_impl;
        template <typename It> struct any_impl;
    };

    //! Return the first element of a non-empty iterable.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp head
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto head = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct head_impl;

    struct _head {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return head_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _head head{};
#endif

    //! Return a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @relates Iterable
    //!
    //! In particular, `tail(iterable)` is functionally equivalent to
    //! `drop(int_<1>, iterable)`.
    //!
    //! ### Example
    //! @snippet example/iterable.cpp tail
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto tail = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct tail_impl;

    struct _tail {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return tail_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _tail tail{};
#endif

    //! Return whether the iterable is empty.
    //! @relates Iterable
    //!
    //! Specificaly, `is_empty` must return a [compile-time]
    //! (@ref Logical_terminology) `Logical` representing
    //! whether the iterable is empty.
    //!
    //! ### Example
    //! @snippet example/iterable.cpp is_empty
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_empty = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct is_empty_impl;

    struct _is_empty {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return is_empty_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _is_empty is_empty{};
#endif

    //! Return the `n`th element of an iterable.
    //! @relates Iterable
    //!
    //! This is functionally equivalent to `head(drop(n, iterable))`.
    //!
    //!
    //! @param n
    //! A non-negative `IntegralConstant` representing the 0-based index of
    //! the element to return. The iterable must contain at least `n + 1`
    //! elements.
    //!
    //! @param iterable
    //! The iterable in which an element is fetched.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp at
    //!
    //! ### Benchmarks
    //! @image html benchmark/iterable/at.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto at = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct at_impl;

    struct _at {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return at_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _at at{};
#endif

    //! Equivalent to `at`; provided for convenience.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto at_c = [](auto&& iterable) -> decltype(auto) {
        return at(size_t<n>, forwarded(iterable));
    };
#else
    template <detail::std::size_t n>
    struct _at_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return at(size_t<n>, detail::std::forward<Xs>(xs)); }
    };

    template <detail::std::size_t n>
    constexpr _at_c<n> at_c{};
#endif

    //! Return the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! This is functionally equivalent to
    //! @code
    //!     at(pred(length(iterable)), iterable)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/iterable.cpp last
    //!
    //! ### Benchmarks
    //! @image html benchmark/iterable/last.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto last = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct last_impl;

    struct _last {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return last_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _last last{};
#endif

    //! Drop the first `n` elements of an iterable and return the rest.
    //! @relates Iterable
    //!
    //!
    //! @param n
    //! A non-negative `IntegralConstant` representing the number of elements
    //! to be dropped from the iterable. If `n` is greater than the number of
    //! elements in the iterable, the returned iterable is empty.
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop
    //!
    //! ### Benchmarks
    //! @image html benchmark/iterable/drop.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_impl;

    struct _drop {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return drop_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _drop drop{};
#endif

    //! Equivalent to `drop`; provided for convenience.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto drop_c = [](auto&& iterable) -> decltype(auto) {
        return drop(size_t<n>, forwarded(iterable));
    };
#else
    template <detail::std::size_t n>
    struct _drop_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return drop(size_t<n>, detail::std::forward<Xs>(xs)); }
    };

    template <detail::std::size_t n>
    constexpr _drop_c<n> drop_c{};
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is not satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //! If the iterable is not finite, the `predicate` has to return a false-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! be dropped from the structure. In the current version of the library,
    //! `predicate` should return a [compile-time](@ref Logical_terminology)
    //! `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop_while
    //!
    //! ### Benchmarks
    //! @image html benchmark/iterable/drop_while.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_while = [](auto&& iterable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_while_impl;

    struct _drop_while {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return drop_while_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _drop_while drop_while{};
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_until` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is satisfied.
    //! If the iterable is not finite, the `predicate` has to return a true-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //! @note
    //! This is effectively equivalent to `drop_while` with a negated
    //! `predicate`.
    //!
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` and
    //! subsequent elements should be kept in the structure. In the current
    //! version of the library, `predicate` should return a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop_until
    //!
    //! ### Benchmarks
    //! @image html benchmark/iterable/drop_until.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_until = [](auto&& iterable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_until_impl;

    struct _drop_until {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return drop_until_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _drop_until drop_until{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ITERABLE_HPP
