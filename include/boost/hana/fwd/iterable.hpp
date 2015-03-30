/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ITERABLE_HPP
#define BOOST_HANA_FWD_ITERABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/core/operators.hpp>


namespace boost { namespace hana {
    namespace operators {
        template <typename Derived>
        struct Iterable_ops;
    }

    //! @ingroup group-concepts
    //! The `Iterable` concept represents data structures supporting external
    //! iteration.
    //!
    //! Intuitively, an Iterable can be seen as a kind of container whose
    //! elements can be pulled out one at a time. An Iterable also provides
    //! a way to know when the _container_ is empty, i.e. when there are no
    //! more elements to pull out.
    //!
    //! Iterables are also Foldable; whereas Foldable represents data
    //! structures supporting internal iteration with the ability to
    //! accumulate a result, the Iterable concept allows inverting the
    //! control of the iteration. This is more flexible than Foldable
    //! since it allows e.g. infinite structures to be iterated only in
    //! part, while trying to fold such a structure would never finish.
    //!
    //! @todo
    //! There's a problem; Foldable is more general than Iterable, but there
    //! are some infinite Iterables that can't be folded properly. So both
    //! concepts actually just overlap and there is no superclass relation?
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `head`, `tail` and `is_empty`
    //!
    //!
    //! Laws
    //! ----
    //! The laws that must be respected by models of the Iterable concept
    //! essentially make sure that external iteration and internal iteration
    //! over a data structure are well-behaved. Hence, we request that the
    //! models for Iterable and Foldable are consistent. First, let's define
    //! the notion of a _linearization_. For a Foldable data structure `xs`,
    //! the linearization of `xs` is the sequence of all the elements in the
    //! structure, as-if they had been put in a list:
    //! @code
    //!     linearization(xs) = [x1, x2, ..., xn]
    //! @endcode
    //!
    //! Note that it is always possible to produce such a linearization for a
    //! finite Foldable by setting
    //! @code
    //!     linearization(xs) = fold.left(xs, [], prepend)
    //! @endcode
    //! for an appropriate type of Sequence (heterogeneous or not).
    //!
    //! We can now express the laws of Iterable in terms of the linearization.
    //! For any finite Iterable `xs` with linearization `[x1, ..., xn]`, the
    //! following laws must be satisfied:
    //! @code
    //!     head(it) == x1
    //!
    //!     linearization(tail(it)) == tail(linearization(it))
    //!                             == [x2, ..., xn]
    //!
    //!     is_empty(it)  <=>  is_empty(linearization(it))
    //!                   <=>  n == 0
    //! @endcode
    //!
    //! This says that linearizing an Iterable and then iterating through it
    //! should be equivalent to iterating through it in the first place. With
    //! the definition of linearization given above, this forces the models of
    //! Foldable and Iterable to be consistent.
    //!
    //!
    //! Superclasses
    //! ------------
    //! 1. `Foldable` (model provided)\n
    //! Every finite Iterable gives rise to a Foldable, and a model of
    //! Foldable is provided for Iterables via the `Iterable::fold_right_impl`
    //! and `Iterable::fold_left_impl` methods.
    //!
    //! Let `xs` be an Iterable and let `xi` denote the `i`-th element in its
    //! linearization. In other words, `xs` can be linearized as
    //! `[x1, ..., xN]`, where `N` is the number of elements in `xs`. Then,
    //! right-folding `xs` with a binary operation `*` (in infix notation for
    //! legibility) is equivalent to
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
    //! this makes no difference. Also note that folds with an initial state
    //! are implemented in an analogous way, and they are provided as
    //! `Iterable::fold_{left,right}_nostate_impl`.
    //!
    //! 2. `Searchable` (model provided)\n
    //! An Iterable can be searched by doing a linear search through the
    //! elements, with the keys and values both being the elements in the
    //! iterable.
    //! @snippet example/iterable.cpp Searchable
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operator is provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     xs[n]  ->  at(n, xs)
    //! @endcode
    //! To take advantage of this operator for a type `T`, `T` must inherit
    //! `hana::operators::Iterable_ops<T>`.
    //!
    //!
    //! @todo
    //! - Add perfect forwarding in the methods.
    //! - Use perfect forwarding in `Iterable::find_impl` once Clang
    //!   bug #20619 is fixed.
    struct Iterable {
        template <typename It> struct fold_left_impl;
        template <typename It> struct fold_right_impl;
        template <typename It> struct fold_left_nostate_impl;
        template <typename It> struct fold_right_nostate_impl;

        template <typename It> struct find_if_impl;
        template <typename It> struct any_of_impl;
    };

    //! Returns the first element of a non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `head(xs)` is equivalent to `x1`. If `xs` is empty, it is an error to
    //! use `head`.
    //!
    //!
    //! Example
    //! -------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::head(xs) requires xs to be an Iterable");
#endif
            return head_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _head head{};
#endif

    //! Returns a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `tail(xs)` is an Iterable of the same data type whose linearization is
    //! `[x2, ..., xN]`. In particular, `tail(xs)` is functionally equivalent
    //! to `drop(size_t<1>, xs)`.
    //!
    //!
    //! Example
    //! -------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::tail(xs) requires xs to be an Iterable");
#endif
            return tail_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _tail tail{};
#endif

    //! Returns whether the iterable is empty.
    //! @relates Iterable
    //!
    //! `is_empty` must return a compile-time `Logical` representing
    //! whether the iterable is empty.
    //!
    //!
    //! Example
    //! -------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::is_empty(xs) requires xs to be an Iterable");
#endif

            return is_empty_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _is_empty is_empty{};
#endif

    //! Returns the `n`th element of an iterable.
    //! @relates Iterable
    //!
    //! Given a `Constant` index and an iterable, `at` returns the
    //! element located at the index in the linearization of the iterable.
    //! Specifically, given an iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `at(k, xs)` is equivalent to `xk`.
    //!
    //!
    //! @param n
    //! A (non-negative) `Constant` of an unsigned integral type representing
    //! the 0-based index of the element to return. It is an error to call
    //! `at` with an index that is either out of bounds for the iterable,
    //! not of an unsigned type or not a Constant.
    //!
    //! @param iterable
    //! The iterable in which an element is retrieved. The iterable must
    //! contain at least `n + 1` elements.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp at
    //!
    //!
    //! Benchmarks
    //! ----------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::at(n, xs) requires xs to be an Iterable");
#endif
            return at_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _at at{};
#endif

    //! Equivalent to `at`; provided for convenience.
    //! @relates Iterable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto at_c = [](auto&& iterable) -> decltype(auto) {
        return at(size_t<n>, forwarded(iterable));
    };
#else
    template <detail::std::size_t n>
    struct _at_c;

    template <detail::std::size_t n>
    constexpr _at_c<n> at_c{};
#endif

    //! Returns the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty and finite iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `last(xs)` is equivalent to `xN`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp last
    //!
    //!
    //! Benchmarks
    //! ----------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::last(xs) requires xs to be an Iterable");
#endif
            return last_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _last last{};
#endif

    //! Drops the first `n` elements of an iterable and returns the rest.
    //! @relates Iterable
    //!
    //! Given an (non-negative) `Constant` `n` of an unsigned integral type
    //! and an iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `drop(n, xs)` is an iterable of the same data type whose
    //! linearization is `[xn+1, ..., xN]`. In particular, note that
    //! this method does not mutate the original iterable in any way.
    //!
    //! @param n
    //! A non-negative `Constant` of an unsigned integral type representing
    //! the number of elements to be dropped from the iterable. If `n` is
    //! greater than the number of elements in the iterable, the returned
    //! iterable is empty.
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/iterable/drop.ctime.png
    //!
    //! @todo
    //! Provide two variants; `drop.at_most` and `drop.exactly`.
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::drop(n, xs) requires xs to be an Iterable");
#endif
            return drop_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _drop drop{};
#endif

    //! Equivalent to `drop`; provided for convenience.
    //! @relates Iterable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto drop_c = [](auto&& iterable) -> decltype(auto) {
        return drop(size_t<n>, forwarded(iterable));
    };
#else
    template <detail::std::size_t n>
    struct _drop_c;

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
    //! `predicate` should return a compile-time `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_while
    //!
    //!
    //! Benchmarks
    //! ----------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::drop_while(xs, pred) requires xs to be an Iterable");
#endif
            return drop_while_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
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
    //! version of the library, `predicate` should return a compile-time
    //! `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_until
    //!
    //!
    //! Benchmarks
    //! ----------
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Iterable, typename datatype<Xs>::type>{},
            "hana::drop_until(xs, pred) requires xs to be an Iterable");
#endif
            return drop_until_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _drop_until drop_until{};
#endif

    template <>
    struct operators::of<Iterable>
        : decltype(at)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ITERABLE_HPP
