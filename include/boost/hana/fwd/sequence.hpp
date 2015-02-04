/*!
@file
Forward declares `boost::hana::Sequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SEQUENCE_HPP
#define BOOST_HANA_FWD_SEQUENCE_HPP

namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Sequence` concept represents generic index-based sequences.
    //!
    //!
    //! ...
    //!
    //!
    //! Laws
    //! ----
    //! ...
    //!
    //!
    //! Superclasses
    //! ------------
    //! 1. `Comparable` (model provided)\n
    //! Two `Sequence`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //! @snippet example/sequence.cpp comparable
    //!
    //! 2. `Orderable`\n
    //! @todo
    //!
    //! 3. `Foldable`, `Iterable`, `Searchable`\n
    //! @todo
    //!
    //! 4. `Traversable` (model provided)\n
    //! @todo
    //! @snippet example/sequence.cpp traversable
    //!
    //! 5. `Functor`\n
    //! `List`s implement `transform` as the mapping of a function over each
    //! element of the list, which is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //! @snippet example/list/functor.cpp transform
    //!
    //! 6. `Applicative`\n
    //! A value can be lifted into a singleton sequence with `lift`. `ap(fs, xs)`
    //! applies each function in the list `fs` to each value in the list `xs`,
    //! and returns a list containing all the results.
    //! @snippet example/list/applicative.cpp main
    //!
    //! 7. `Monad`\n
    //! A function returning a list of results can be mapped over all the
    //! elements of a list and have all the results concatenated using `bind`.
    //! Also, a list of lists can be flattened one level with `flatten`.
    //! @snippet example/list/monad.cpp main
    //!
    //! 8. `MonadPlus`\n
    //! @todo
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. ...
    //! ...
    //!
    //!
    //! @todo
    //! - Implement the following methods:
    //!     - `intercalate`, `transpose`, `subsequences`
    //!     - `split_at`, `break`, `inits`, `tails`
    //!     - `iterate`
    //! - Consider implementing the following methods:
    //!     - `nub_by`, `nub`, `delete_by`, `insert`
    //!     - `set_difference_by`, `set_union_by`, `set_intersection_by`
    //! - Since we can benchmark the isomorphic instances, put the benchmarks
    //!   in the documentation.
    struct Sequence {
        template <typename T, typename U>
        struct equal_impl;

        template <typename T>
        struct traverse_impl;
    };

    //! Group the elements of a sequence into subgroups of adjacent elements
    //! that are "equal" with respect to a predicate.
    //! @relates Sequence
    //!
    //! Specifically, `group_by` takes a sequence and returns a sequence of
    //! sequences such that the concatenation of the result is equal to the
    //! argument. Moreover, each subsequence contains only elements for which
    //! the predicate is satisfied when applied to two adjacent elements.
    //!
    //!
    //! @param predicate
    //! A binary function called as `predicate(x, y)`, where `x` and `y`
    //! are _adjacent_ elements in the sequence, and returning a `Logical`
    //! representing whether both elements should be in the same group
    //! (subsequence) of the result. The result returned by `predicate` has
    //! to be a `Constant Logical`. Also, `predicate` has to define an
    //! equivalence relation as defined by the `Comparable` concept.
    //!
    //! @param xs
    //! The sequence to split into groups.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp group_by
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/group_by.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto group_by = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct group_by_impl;

    struct _group_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
            return group_by_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _group_by group_by{};
#endif

    //! Group the elements of a sequence into subsequences of equal
    //! adjacent elements.
    //! @relates Sequence
    //!
    //! Specifically, `group(xs)` is equivalent to `group_by(equal, xs)`.
    //! For this method to work, comparing adjacent elements have to return
    //! a `Constant Logical`.
    //!
    //! @param xs
    //! The sequence to split into groups.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp group
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto group = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct group_impl;

    struct _group {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return group_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _group group{};
#endif

    //! Remove the last element of a non-empty sequence.
    //! @relates Sequence
    //!
    //! If the sequence is empty, a compile-time assertion is triggered.
    //! Otherwise, a copy of the original sequence with its last element
    //! removed is returned.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp init
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto init = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct init_impl;

    struct _init {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return init_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _init init{};
#endif

    //! Insert a value between each pair of elements in a sequence.
    //! @relates Sequence
    //!
    //! Specifically, `intersperse([x1, x2, ..., xn], z)` is a sequence
    //! equivalent to `[x1, z, x2, z, x3, ..., xn-1, z, xn]`. If the sequence
    //! is empty or has a single element, then `intersperse` returns the
    //! sequence as-is.
    //!
    //!
    //! @param xs
    //! The sequence in which a value is interspersed.
    //!
    //! @param z
    //! The value to be interspersed in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp intersperse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto intersperse = [](auto&& xs, auto&& z) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct intersperse_impl;

    struct _intersperse {
        template <typename Xs, typename Z>
        constexpr decltype(auto) operator()(Xs&& xs, Z&& z) const {
            return intersperse_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Z>(z)
            );
        }
    };

    constexpr _intersperse intersperse{};
#endif

    //! Partition a sequence based on a `predicate`.
    //! @relates Sequence
    //!
    //! Specifically, returns an unspecified `Product` whose first element is
    //! a sequence of the elements satisfying the predicate, and whose second
    //! element is a sequence of the elements that do not satisfy the predicate.
    //!
    //!
    //! @param xs
    //! The sequence to be partitioned.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` in the
    //! sequence and returning a `Constant Logical`. If the result of
    //! `predicate` is true, then `x` is added to the sequence in the first
    //! component of the resulting `Product`. Otherwise, `x` is added to the
    //! sequence in the second component.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp partition
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto partition = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct partition_impl;

    struct _partition {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return partition_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _partition partition{};
#endif

    //! Return a sequence of all the permutations of the given sequence.
    //! @relates Sequence
    //!
    //! Specifically, `permutations(xs)` is a sequence whose elements are
    //! permutations of the original sequence `xs`. The permutations are not
    //! guaranteed to be in any specific order. Also note that the number
    //! of permutations grows very rapidly as the length of the original
    //! sequence increases. The growth rate is `O(length(xs)!)`; with a
    //! sequence `xs` of length only 8, `permutations(xs)` contains over
    //! 40 000 elements!
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp permutations
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/permutations.ctime.png
    //!
    //!
    //! @todo
    //! When the size of the sequence is known at compile-time, we can
    //! generate the indices `constexpr` and then simply use `at_c`.
    //! This could vastly improve the compilation time.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto permutations = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct permutations_impl;

    struct _permutations {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return permutations_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _permutations permutations{};
#endif

    //! Remove the element at a given index from a sequence.
    //! @relates Sequence
    //!
    //! `remove_at` returns a new sequence identical to the original, except
    //! that the element at the given index is removed. Specifically,
    //! `remove_at(n, [x0, ..., xn-1, xn, xn+1, ..., xm])` is a new
    //! sequence equivalent to `[x0, ..., xn-1, xn+1, ..., xm]`.
    //!
    //!
    //! @param n
    //! An `IntegralConstant` representing the index of the element to be
    //! removed from the sequence.
    //!
    //! @param xs
    //! A sequence from which an element is to be removed.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp remove_at
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/remove_at.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto remove_at = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct remove_at_impl;

    struct _remove_at {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return remove_at_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _remove_at remove_at{};
#endif

    //! Equivalent to `remove_at`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp remove_at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto remove_at_c = [](auto&& xs) -> decltype(auto) {
        return remove_at(size_t<n>, forwarded(xs));
    };
#else
    template <detail::std::size_t n>
    struct _remove_at_c;

    template <detail::std::size_t n>
    constexpr _remove_at_c<n> remove_at_c{};
#endif

    //! Reverse a sequence.
    //! @relates Sequence
    //!
    //! Specifically, `reverse(xs)` is a new sequence containing the same
    //! elements as `xs`, except in reverse order.
    //!
    //!
    //! @param xs
    //! The sequence to reverse.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp reverse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct reverse_impl;

    struct _reverse {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return reverse_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _reverse reverse{};
#endif

    //! Similar to `foldl`, but returns a sequence of reduced values from
    //! the left.
    //! @relates Sequence
    //!
    //! `scanl` folds a sequence from the left, like `foldl`, but it builds
    //! up a sequence of the intermediary results computed along the way
    //! and returns that instead. Specifically, the `i`th element of the
    //! `scanl([x1, ..., xn], state, f)` sequence is equivalent to
    //! `foldl([x1, ..., xi], state, f)`. For example, consider this left
    //! fold on a short sequence:
    //! @code
    //!     foldl([x1, x2, x3], state, f) == f(f(f(state, x1), x2), x3)
    //! @endcode
    //! The analogous sequence generated with `scanl` will be
    //! @code
    //!     scanl([x1, x2, x3], state, f) == [
    //!         state,
    //!         f(state, x1),
    //!         f(f(state, x1), x2),
    //!         f(f(f(state, x1), x2), x3)
    //!     ]
    //! @endcode
    //!
    //!
    //! @param xs
    //! The sequence to fold from the left.
    //!
    //! @param state
    //! The initial value used for folding. This will always be the first
    //! element of the resulting sequence.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp scanl
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/scanl.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanl = [](auto&& xs, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct scanl_impl;

    struct _scanl {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            return scanl_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanl scanl{};
#endif

    //! Variant of `scanl` that has no starting value argument.
    //! @relates Sequence
    //!
    //! `scanl1` is to `scanl` what `foldl1` is to `foldl`. Like `scanl`,
    //! `scanl1` folds a sequence from the left and build up a sequence
    //! of the intermediary results. However, it uses the first element
    //! of the sequence as an initial state instead of taking it as an
    //! argument. For example, consider this left fold on a short sequence:
    //! @code
    //!     foldl1([x1, x2, x3, x4], f) == f(f(f(x1, x2), x3), x4)
    //! @endcode
    //! The analogous sequence generated with `scanl1` will be
    //! @code
    //!     scanl1([x1, x2, x3, x4], f) == [
    //!         x1,
    //!         f(x1, x2),
    //!         f(f(x1, x2), x3),
    //!         f(f(f(x1, x2), x3), x4)
    //!     ]
    //! @endcode
    //!
    //! @note
    //! Unlike `foldl1`, which fails when it is applied to an empty structure,
    //! `scanl1` can be called on an empty sequence, in which case it will
    //! simply return an empty sequence.
    //!
    //!
    //! @param xs
    //! The sequence to fold from the left.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp scanl1
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/scanl1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanl1 = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct scanl1_impl;

    struct _scanl1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return scanl1_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanl1 scanl1{};
#endif

    //! Similar to `foldr`, but returns a sequence of reduced values from
    //! the right.
    //! @relates Sequence
    //!
    //! `scanr` folds a sequence from the right, like `foldr`, but it builds
    //! up a sequence of the intermediary results computed along the way
    //! and returns that instead. For example, consider this right fold on
    //! a short sequence:
    //! @code
    //!     foldr([x1, x2, x3], state, f) == f(x1, f(x2, f(x3, state)))
    //! @endcode
    //! The analogous sequence generated with `scanr` will be
    //! @code
    //!     scanr([x1, x2, x3], state, f) == [
    //!         f(x1, f(x2, f(x3, state))),
    //!               f(x2, f(x3, state)),
    //!                     f(x3, state),
    //!                           state
    //!     ]
    //! @endcode
    //!
    //!
    //! @param xs
    //! The sequence to fold from the right.
    //!
    //! @param state
    //! The initial value used for folding. This will always be the last
    //! element of the resulting sequence.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp scanr
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/scanr.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanr = [](auto&& xs, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct scanr_impl;

    struct _scanr {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            return scanr_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanr scanr{};
#endif



    //! Variant of `scanr` that has no starting value argument.
    //! @relates Sequence
    //!
    //! `scanr1` is to `scanr` what `foldr1` is to `foldr`. Like `scanr`,
    //! `scanr1` folds a sequence from the right and build up a sequence
    //! of the intermediary results. However, it uses the last element
    //! of the sequence as an initial state instead of taking it as an
    //! argument. For example, consider this right fold on a short sequence:
    //! @code
    //!     foldr1([x1, x2, x3, x4], f) == f(x1, f(x2, f(x3, x4)))
    //! @endcode
    //! The analogous sequence generated with `scanr1` will be
    //! @code
    //!     scanr1([x1, x2, x3, x4], f) == [
    //!         f(x1, f(x2, f(x3, x4))),
    //!               f(x2, f(x3, x4)),
    //!                     f(x3, x4),
    //!                           x4
    //!     ]
    //! @endcode
    //!
    //! @note
    //! Unlike `foldr1`, which fails when it is applied to an empty structure,
    //! `scanr1` can be called on an empty sequence, in which case it will
    //! simply return an empty sequence.
    //!
    //!
    //! @param xs
    //! The sequence to fold from the right.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp scanr1
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/scanr1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanr1 = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct scanr1_impl;

    struct _scanr1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return scanr1_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanr1 scanr1{};
#endif

    //! Extract a subsequence delimited by the given indices.
    //! @relates Sequence
    //!
    //! Specifically, `slice(xs, from, to)` is a sequence containing all the
    //! elements of `xs` at indices in the half-open interval delimited by
    //! [`from`, `to`). Note that the indices are 0-based. For this operation
    //! to be valid, `xs` must contain at least `to + 1` elements, and it must
    //! be true that `from <= to`.
    //!
    //!
    //! @param xs
    //! The sequence to slice.
    //!
    //! @param from
    //! The index of the first element in the slice. `from` must be a
    //! non-negative `IntegralConstant`.
    //!
    //! @param to
    //! One-past the index of the last element in the slice. `to` must be
    //! a non-negative `IntegralConstant` such that `from <= to`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp slice
    //!
    //! @todo
    //! Should this be `slice(xs, from, length)` instead?
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto slice = [](auto&& xs, auto&& from, auto&& to) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct slice_impl;

    struct _slice {
        template <typename Xs, typename From, typename To>
        constexpr decltype(auto) operator()(Xs&& xs, From&& from, To&& to) const {
            return slice_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<From>(from),
                detail::std::forward<To>(to)
            );
        }
    };

    constexpr _slice slice{};
#endif

    //! Equivalent to `slice`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp slice_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t from, std::size_t to>
    constexpr auto slice_c = [](auto&& xs) -> decltype(auto) {
        return slice(forwarded(xs), size_t<from>, size_t<to>);
    };
#else
    template <detail::std::size_t from, detail::std::size_t to>
    struct _slice_c;

    template <detail::std::size_t from, detail::std::size_t to>
    constexpr _slice_c<from, to> slice_c{};
#endif

    //! Sort a sequence based on the given `predicate`.
    //! @relates Sequence
    //!
    //! Specifically, `sort_by` returns a new sequence containing the same
    //! elements as the original, except they are ordered in such a way
    //! that if `x` comes before `y` in the sequence, then either
    //! `predicate(x, y)` is true, or both `predicate(x, y)` and
    //! `predicate(y, x)` are false. Also note that the sort is guaranteed
    //! to be stable. Hence, if `x` comes before `y` in the original sequence
    //! and both `predicate(x, y)` and `predicate(y, x)` are false, then
    //! `x` will come before `y` in the resulting sequence.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)` for two elements `x` and `y` of
    //! the sequence, and returning a `Logical` representing whether `x` should
    //! appear __before__ `y` in the resulting sequence. More specifically,
    //! `predicate` must define a [strict weak ordering][1] on the elements of
    //! the sequence. In the current version of the library, also note that
    //! `predicate` must return a `Constant Logical` when called with any two
    //! elements of the sequence.
    //!
    //! @param xs
    //! The sequence to sort.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp sort_by
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sort_by = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct sort_by_impl;

    struct _sort_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
            return sort_by_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _sort_by sort_by{};
#endif

    //! Sort a sequence based on the total order induced by the `less` method.
    //! @relates Sequence
    //!
    //! The elements in the sequence must all be `Orderable`. Also, in the
    //! current version of the library, the `less` method must return a
    //! `Constant Logical` when called with any two elements of the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp sort
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/sort.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sort = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct sort_impl;

    struct _sort {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return sort_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _sort sort{};
#endif

    //! Returns a `Product` containing the longest prefix of a sequence
    //! satisfying a predicate, and the rest of the sequence.
    //! @relates Sequence
    //!
    //! The first component of the returned `Product` is a sequence for which
    //! all elements satisfy the given predicate. The second component of the
    //! returned `Product` is a sequence containing the remainder of the
    //! argument. Both or either sequences may be empty, depending on the
    //! input argument. Specifically, `span(xs, predicate)` is equivalent to
    //! @code
    //!     span(xs, predicate) == pair(take_while(xs, predicate),
    //!                                 drop_while(xs, predicate))
    //! @endcode
    //! except that `pair` may be an arbitrary `Product`.
    //!
    //!
    //! @param xs
    //! The sequence to break into two parts.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical. In the current implementation of
    //! the library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp span
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto span = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct span_impl;

    struct _span {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return span_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _span span{};
#endif

    //! Return the first `n` elements of a sequence.
    //! @relates Sequence
    //!
    //! Specifically, `take(n, xs)` is a new sequence containing the first
    //! `n` elements of `xs`, in the same order.
    //!
    //!
    //! @param n
    //! A non-negative `Integral` representing the number of elements to keep
    //! in the resulting sequence. If `n` is greater than the length of the
    //! input sequence, the whole sequence is returned.
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/take.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_impl;

    struct _take {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return take_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _take take{};
#endif

    //! Equivalent to `take`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto take_c = [](auto&& xs) -> decltype(auto) {
        return take(size_t<n>, forwarded(xs));
    };
#else
    template <detail::std::size_t n>
    struct _take_c;

    template <detail::std::size_t n>
    constexpr _take_c<n> take_c{};
#endif

    //! Take elements from a sequence until the `predicate` is satisfied.
    //! @relates Sequence
    //!
    //! Specifically, `take_until` returns a new sequence containing the
    //! longest prefix of `xs` in which all elements do not satisfy the
    //! predicate. This is effectively equivalent to `take_while` with a
    //! negated predicate.
    //!
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical` representing whether the resulting
    //! sequence should stop at the element before `x`. In the current version
    //! of the library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_until
    //!
    //! Benchmarks
    //! -------
    //! @image html benchmark/sequence/take_until.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_until = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_until_impl;

    struct _take_until {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return take_until_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _take_until take_until{};
#endif

    //! Take elements from a sequence while the `predicate` is satisfied.
    //! @relates Sequence
    //!
    //! Specifically, `take_while` returns a new sequence containing the
    //! longest prefix of `xs` in which all the elements satisfy the given
    //! predicate.
    //!
    //!
    //! @param xs
    //! The sequence to take elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical` representing whether `x` should be
    //! included in the resulting sequence. In the current version of the
    //! library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_while
    //!
    //!
    //! Benchmarks
    //! -------
    //! @image html benchmark/sequence/take_while.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_while = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_while_impl;

    struct _take_while {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return take_while_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _take_while take_while{};
#endif

    //! Dual to `foldl` for sequences.
    //! @relates Sequence
    //!
    //! While `foldl` reduces a structure to a summary value, `unfoldl` builds
    //! a sequence from a seed value and a function. In some cases, `unfoldl`
    //! can undo a `foldl` operation:
    //! @code
    //!     unfoldl(g, foldl(xs, z, f))
    //! @endcode
    //!
    //! if the following holds
    //! @code
    //!     g(f(y, x)) == just(pair(y, x))
    //!     g(z) == nothing
    //! @endcode
    //!
    //!
    //! @tparam S
    //! The data type of the sequence to build up.
    //!
    //! @param f
    //! A function called as `f(init)`, where `init` is an initial value,
    //! and returning
    //!     1. `nothing` if it is done producing the sequence.
    //!     2. `just(pair(init, x))` if it isn't, where `init` is the new
    //!        initial value used in the next call to `f` and `x` is an
    //!        element to be appended to the resulting sequence. Also note
    //!        that `pair` may actually be replaced by any `Product`.
    //!
    //! @param init
    //! An initial value to build the sequence from.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp unfoldl
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename S>
    constexpr auto unfoldl = [](auto&& f, auto&& init) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct unfoldl_impl;

    template <typename L>
    struct _unfoldl {
        template <typename F, typename Initial>
        constexpr decltype(auto) operator()(F&& f, Initial&& initial) const {
            return unfoldl_impl<L>::apply(
                detail::std::forward<decltype(f)>(f),
                detail::std::forward<decltype(initial)>(initial)
            );
        }
    };

    template <typename L>
    constexpr _unfoldl<L> unfoldl{};
#endif

    //! Dual to `foldr` for sequences.
    //! @relates Sequence
    //!
    //! While `foldr` reduces a structure to a summary value, `unfoldr` builds
    //! a sequence from a seed value and a function. In some cases, `unfoldr`
    //! can undo a `foldr` operation:
    //! @code
    //!     unfoldr(g, foldr(xs, z, f))
    //! @endcode
    //!
    //! if the following holds
    //! @code
    //!     g(f(x, y)) == just(pair(x, y))
    //!     g(z) == nothing
    //! @endcode
    //!
    //!
    //! @tparam S
    //! The data type of the sequence to build up.
    //!
    //! @param f
    //! A function called as `f(init)`, where `init` is an initial value,
    //! and returning
    //!     1. `nothing` if it is done producing the sequence.
    //!     2. `just(pair(x, init))` if it isn't, where `init` is the new
    //!        initial value used in a recursive call to `f` and `x` is an
    //!        element prepended to the resulting sequence. Also note that
    //!        `pair` may actually be replaced by any `Product`.
    //!
    //! @param init
    //! An initial value to build the list from.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp unfoldr
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename S>
    constexpr auto unfoldr = [](auto&& f, auto&& init) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct unfoldr_impl;

    template <typename L>
    struct _unfoldr {
        template <typename F, typename Initial>
        constexpr decltype(auto) operator()(F&& f, Initial&& initial) const {
            return unfoldr_impl<L>::apply(
                detail::std::forward<decltype(f)>(f),
                detail::std::forward<decltype(initial)>(initial)
            );
        }
    };

    template <typename L>
    constexpr _unfoldr<L> unfoldr{};
#endif

    //! Unzip a sequence of sequences.
    //! @relates Sequence
    //!
    //! `unzip` can undo a `zip` operation. Specifically, it takes a sequence
    //! of the form
    //! @code
    //!     [s1, s2, ..., sn]
    //! @endcode
    //! where each `si` is a sequence, and returns a sequence equivalent to
    //! `zip(s1, s2, ..., sn)`.
    //!
    //!
    //! @param xs
    //! A sequence of sequences to unzip.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp unzip
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto unzip = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct unzip_impl;

    struct _unzip {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return unzip_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _unzip unzip{};
#endif

    //! Zip one sequence or more.
    //! @relates Sequence
    //!
    //! This is equivalent to `zip_with(make<S>, xs, ys...)`, where `S` is
    //! the data type of `xs`.
    //!
    //! @note
    //! All the sequences must have the same data type, and only the data type
    //! of the first one is used for tag-dispatching.
    //!
    //!
    //! @param xs, ys...
    //! The sequences to zip together.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp zip
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto zip = [](auto&& xs, auto&& ...ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct zip_impl;

    struct _zip {
        template <typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ...ys) const {
            return zip_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)...
            );
        }
    };

    constexpr _zip zip{};
#endif

    //! Zip one sequence or more with a given function.
    //! @relates Sequence
    //!
    //! Specifically, returns a sequence whose i-th element is `f(s1[i], ..., sn[i])`,
    //! where `sk[i]` denotes the i-th element of the k-th sequence passed as
    //! an argument. The returned sequence stops when the shortest input sequence
    //! is exhausted.
    //!
    //! In other words, `zip_with(f, s1, ..., sn)` is a sequence of the form
    //! @code
    //!     zip_with(f, s1, ..., sn) == [
    //!         f(s1[0], ..., sn[0]),
    //!         f(s1[1], ..., sn[1]),
    //!         ...,
    //!         f(s1[k], ..., sn[k])
    //!     ]
    //! @endcode
    //! where `k` is the length of the shortest sequence.
    //!
    //! @note
    //! All the sequences must have the same data type, and only the data type
    //! of the first one is used for tag-dispatching.
    //!
    //!
    //! @param f
    //! A function called as `f(s1[k], ..., sn[k])`, where `si[k]` are
    //! elements of the zipped sequences as explained above.
    //!
    //! @param xs, ys...
    //! The sequences to zip with the given function.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp zip_with
    //!
    //!
    //! Benchmarks
    //! -------
    //! @image html benchmark/sequence/zip_with.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto zip_with = [](auto&& f, auto&& xs, auto&& ...ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct zip_with_impl;

    struct _zip_with {
        template <typename F, typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(F&& f, Xs&& xs, Ys&& ...ys) const {
            return zip_with_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<F>(f),
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)...
            );
        }
    };

    constexpr _zip_with zip_with{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SEQUENCE_HPP
