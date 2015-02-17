/*!
@file
Forward declares `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_LIST_HPP
#define BOOST_HANA_FWD_LIST_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! General purpose index-based sequence.
    //!
    //!
    //! ### Laws
    //! For any two `List`s `xs` and `ys`, the following statement must hold:
    //!
    //! @code
    //!     xs == ys if and only if to<Tuple>(xs) == to<Tuple>(ys)
    //! @endcode
    //!
    //! This is basically saying that all `List` instances are isomorphic to
    //! the instance defined by the `Tuple` data type, and it therefore makes
    //! sense to define comparison for any two instances of `List`.
    //!
    //!
    //! @todo
    //! - How to implement iterate and repeat?
    //! - There is a strong relationship between this and `MonadPlus`.
    //!   Actually, they might be just the same. Check this out.
    //! - Implement the following methods:
    //!     - `intercalate`, `transpose`, `subsequences`
    //!     - `split_at`, `break`, `inits`, `tails`
    //! - Consider implementing the following methods:
    //!     - `nub_by`, `nub`, `delete_by`, `insert`
    //!     - `set_difference_by`, `set_union_by`, `set_intersection_by`
    //! - Since we can benchmark the isomorphic instances, put the benchmarks
    //!   in the documentation.
    struct List {
        BOOST_HANA_TYPECLASS(List);
        template <typename T>
        struct mcd;
    };

    //! Group the elements of a list into subgroups of adjacent elements that
    //! are "equal" with respect to a predicate.
    //! @relates List
    //!
    //! Specifically, `group_by` takes a list and returns a list of lists
    //! such that the concatenation of the result is equal to the argument.
    //! Moreover, each sublist contains only elements for which the predicate
    //! is satisfied when applied to two adjacent elements.
    //!
    //!
    //! @param predicate
    //! A binary function called as `predicate(x, y)`, where `x` and `y`
    //! are _adjacent_ elements in the list, and returning a `Logical`
    //! representing whether both elements should be in the same group
    //! (sublist) of the result. The result returned by `predicate` has
    //! to be a [compile-time](@ref Logical_terminology) `Logical`. Also,
    //! `predicate` has to define an [equivalence relation]
    //! (@ref equivalence_relation) as defined by the `Comparable` type class.
    //!
    //! @param xs
    //! The list to split into groups.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp group_by
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/group_by.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto group_by = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _group_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::group_by_impl(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _group_by group_by{};
#endif

    //! Group the elements of a list into subgroups of adjacent equal elements.
    //! @relates List
    //!
    //! Specifically, `group(xs)` is equivalent to `group_by(equal, xs)`.
    //! For this method to work, comparing adjacent elements have to return
    //! a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! @param xs
    //! The list to split into groups.
    //!
    //! ### Example
    //! @snippet example/list.cpp group
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto group = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _group {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::group_impl(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _group group{};
#endif

    //! Remove the last element of a non-empty list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list.cpp init
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto init = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _init {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::init_impl(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _init init{};
#endif

    //! Insert a value between each pair of elements in a list.
    //! @relates List
    //!
    //! Specifically, `intersperse([x1, x2, ..., xn], z)` is a list equivalent
    //! to `[x1, z, x2, z, x3, ..., xn-1, z, xn]`. If the list is empty or has
    //! a single element, then `intersperse` returns the list as is.
    //!
    //!
    //! @param xs
    //! The list in which a value is interspersed.
    //!
    //! @param z
    //! The value to be interspersed in the list.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp intersperse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto intersperse = [](auto&& xs, auto&& z) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _intersperse {
        template <typename Xs, typename Z>
        constexpr decltype(auto) operator()(Xs&& xs, Z&& z) const {
            return List::instance<
                datatype_t<Xs>
            >::intersperse_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Z>(z)
            );
        }
    };

    constexpr _intersperse intersperse{};
#endif

    //! Create a `List` with the given elements in it.
    //! @relates List
    //!
    //!
    //! @tparam L
    //! The data type representing the `List` to create. This can be any
    //! instance of the `List` type class.
    //!
    //! @param xs...
    //! The elements to put in the constructed list. The elements will appear
    //! in the resulting list in the same order as passed to `make`.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp make
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/make.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename L>
    struct make_impl<L, when<is_a<List, L>()>> {
        tag-dispatched
    };
#else
    template <typename L>
    struct make_impl<L, when<is_a<List, L>()>> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return List::instance<L>::make_impl(
                detail::std::forward<Xs>(xs)...
            );
        }
    };
#endif

    //! Partition a list based on a `predicate`.
    //! @relates List
    //!
    //! Specifically, returns an unspecified `Product` whose first element is
    //! a list of the elements satisfying the predicate, and whose second
    //! element is a list of the elements that do not satisfy the predicate.
    //!
    //!
    //! @param xs
    //! The list to be partitioned.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` in the list
    //! and returning a `Logical`. If the result of `predicate` is true-valued,
    //! then `x` is added to the list in the first component of the resulting
    //! `Product`. Otherwise, `x` is added to the list in the second component.
    //! In the current version of the library, the `predicate` has to return
    //! a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp partition
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto partition = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _partition {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return List::instance<
                datatype_t<Xs>
            >::partition_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _partition partition{};
#endif

    //! Return a list of all the permutations of the given list.
    //! @relates List
    //!
    //! The permutations are not guaranteed to be in any specific order.
    //!
    //! @note
    //! Implementation taken from http://stackoverflow.com/a/2184129/627587.
    //!
    //! ### Example
    //! @snippet example/list.cpp permutations
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/permutations.ctime.png
    //!
    //! @bug
    //! We got a performance problem here. Generating the permutations of
    //! a list of more than 3 elements takes a long time.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto permutations = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _permutations {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::permutations_impl(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _permutations permutations{};
#endif

    //! Remove the element at a given index from a list.
    //! @relates List
    //!
    //! Specifically, `remove_at(n, [x0, ..., xn-1, xn, xn+1, ..., xm])` is
    //! a new list equivalent to the list `[x0, ..., xn-1, xn+1, ..., xm]`.
    //!
    //!
    //! @param n
    //! An `IntegralConstant` representing the index of the element to be
    //! removed from the list.
    //!
    //! @param xs
    //! A list from which an element is to be removed.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp remove_at
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/remove_at.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto remove_at = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _remove_at {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::remove_at_impl(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _remove_at remove_at{};
#endif

    //! Equivalent to `remove_at`; provided for convenience.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list.cpp remove_at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto remove_at_c = [](auto&& list) -> decltype(auto) {
        return remove_at(size_t<n>, forwarded(list));
    };
#else
    template <detail::std::size_t n>
    struct _remove_at_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return remove_at(size_t<n>, detail::std::forward<Xs>(xs)); }
    };

    template <detail::std::size_t n>
    constexpr _remove_at_c<n> remove_at_c{};
#endif

    //! Reverse a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list.cpp reverse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _reverse {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::reverse_impl(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _reverse reverse{};
#endif

    //! Similar to `foldl`, but returns a list of reduced values from the left.
    //! @relates List
    //!
    //!
    //! @param xs
    //! The list to scan.
    //!
    //! @param state
    //! The initial value used for folding. This will always be the first
    //! element of the resulting list.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the list.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp scanl
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/scanl.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanl = [](auto&& xs, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _scanl {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            return List::instance<
                datatype_t<Xs>
            >::scanl_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanl scanl{};
#endif

    //! Variant of `scanl` that has no starting value argument.
    //! @relates List
    //!
    //! While `foldl1` may not be called with an empty structure, `scanl1`
    //! can be called with an empty list, in which case it will simply return
    //! an empty list.
    //!
    //!
    //! @param xs
    //! The list to scan.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the list.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp scanl1
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/scanl1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanl1 = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _scanl1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return List::instance<
                datatype_t<Xs>
            >::scanl1_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanl1 scanl1{};
#endif

    //! Similar to `foldr`, but returns a list of reduced values from the right.
    //! @relates List
    //!
    //!
    //! @param xs
    //! The list to scan.
    //!
    //! @param state
    //! The initial value used for folding. This will always be the last
    //! element of the resulting list.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the list.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp scanr
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/scanr.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanr = [](auto&& xs, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _scanr {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            return List::instance<
                datatype_t<Xs>
            >::scanr_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanr scanr{};
#endif

    //! Variant of `scanr` that has no starting value argument.
    //! @relates List
    //!
    //! While `foldr1` may not be called with an empty structure, `scanr1`
    //! can be called with an empty list, in which case it will simply return
    //! an empty list.
    //!
    //!
    //! @param xs
    //! The list to scan.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the list.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp scanr1
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/scanr1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scanr1 = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _scanr1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return List::instance<
                datatype_t<Xs>
            >::scanr1_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _scanr1 scanr1{};
#endif

    //! Extract a sublist delimited by the given indices.
    //! @relates List
    //!
    //! Specifically, `slice(xs, from, to)` is a list containing all the
    //! elements of `xs` at indices in the half-open interval delimited by
    //! [`from`, `to`). Note that the indices are 0-based. For this operation
    //! to be valid, `xs` must contain at least `to + 1` elements, and it must
    //! be true that `from <= to`.
    //!
    //!
    //! @param xs
    //! The list to slice.
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
    //! ### Example
    //! @snippet example/list.cpp slice
    //!
    //! @todo
    //! Should this be `slice(xs, from, length)` instead?
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto slice = [](auto&& xs, auto&& from, auto&& to) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _slice {
        template <typename Xs, typename From, typename To>
        constexpr decltype(auto) operator()(Xs&& xs, From&& from, To&& to) const {
            return List::instance<
                datatype_t<Xs>
            >::slice_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<From>(from),
                detail::std::forward<To>(to)
            );
        }
    };

    constexpr _slice slice{};
#endif

    //! Equivalent to `slice`; provided for convenience.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list.cpp slice_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t from, std::size_t to>
    constexpr auto slice_c = [](auto&& xs) -> decltype(auto) {
        return slice(forwarded(xs), size_t<from>, size_t<to>);
    };
#else
    template <detail::std::size_t from, detail::std::size_t to>
    struct _slice_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return slice(detail::std::forward<Xs>(xs), size_t<from>, size_t<to>);
        }
    };

    template <detail::std::size_t from, detail::std::size_t to>
    constexpr _slice_c<from, to> slice_c{};
#endif

    //! Sort a list based on the `less` [strict weak ordering](@ref strict_weak_ordering).
    //! @relates List
    //!
    //! The elements in the list must all be `Orderable`. In the current
    //! version of the library, the `less` method of the `Orderable`
    //! instance must be a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp sort
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/sort.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sort = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _sort {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::sort_impl(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _sort sort{};
#endif

    //! Sort a list based on the given `predicate`.
    //! @relates List
    //!
    //! The sort is guaranteed to be stable.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)` for two elements `x` and `y` of
    //! the list, and returning a `Logical` representing whether `x` should
    //! appear __before__ `y` in the resulting list. More specifically, the
    //! `predicate` must define a [strict weak ordering](@ref strict_weak_ordering)
    //! on the elements of the list. In the current version of the library,
    //! `predicate` must return a [compile-time](@ref Logical_terminology)
    //! `Logical` when called with any two elements of the list.
    //!
    //! @param xs
    //! The list to sort.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp sort_by
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sort_by = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _sort_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::sort_by_impl(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _sort_by sort_by{};
#endif

    //! Return a `Product` containing the longest prefix of a list satisfying
    //! a predicate, and the rest of the list.
    //! @relates List
    //!
    //! The first element of the returned `Product` is a list for which all
    //! elements satisfy the given predicate. The second element of the
    //! returned `Product` is a list containing the remainder of the argument.
    //! Both or either lists may be empty, depending on the input argument.
    //! Specifically, `span(xs, predicate)` is equivalent to
    //! @code
    //!     span(xs, predicate) == pair(
    //!                             take_while(xs, predicate),
    //!                             drop_while(xs, predicate)
    //!                            )
    //! @endcode
    //! except that `pair` may be an arbitrary `Product`.
    //!
    //!
    //! @param xs
    //! The list to break into two parts.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! list, and returning a `Logical. In the current implementation of the
    //! library, `predicate` has to return a [compile-time]
    //! (@ref Logical_terminology) `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp span
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto span = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _span {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return List::instance<
                datatype_t<Xs>
            >::span_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _span span{};
#endif

    //! Return a list containing the first `n` elements of a list.
    //! @relates List
    //!
    //!
    //! @param n
    //! A non-negative `Integral` representing the number of elements to keep
    //! in the resulting list. If `n` is greater than the length of the input
    //! list, the whole list is returned.
    //!
    //! @param xs
    //! The list to take the elements from.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp take
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/take.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _take {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::take_impl(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _take take{};
#endif

    //! Equivalent to `take`; provided for convenience.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list.cpp take_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto take_c = [](auto&& xs) -> decltype(auto) {
        return take(size_t<n>, forwarded(xs));
    };
#else
    template <detail::std::size_t n>
    struct _take_c {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return take(size_t<n>, detail::std::forward<Xs>(xs));
        }
    };

    template <detail::std::size_t n>
    constexpr _take_c<n> take_c{};
#endif

    //! Take elements until the `predicate` is satisfied.
    //! @relates List
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements do not satisfy the predicate. This is effectively equivalent
    //! to `take_while` with a negated predicate.
    //!
    //!
    //! @param xs
    //! The list to take the elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! list, and returning a `Logical` representing whether the resulting
    //! list should stop at the element before `x`. In the current version
    //! of the library, `predicate` has to return a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp take_until
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/take_until.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_until = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _take_until {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return List::instance<
                datatype_t<Xs>
            >::take_until_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _take_until take_until{};
#endif

    //! Take elements while the `predicate` is satisfied.
    //! @relates List
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements satisfy the given predicate.
    //!
    //!
    //! @param xs
    //! The list to take elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! list, and returning a `Logical` representing whether `x` should be
    //! included in the resulting list. In the current version of the library,
    //! `predicate` has to return a [compile-time](@ref Logical_terminology)
    //! `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp take_while
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/take_while.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_while = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _take_while {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return List::instance<
                datatype_t<Xs>
            >::take_while_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _take_while take_while{};
#endif

    //! Dual to `foldl` for lists.
    //! @relates List
    //!
    //! While `foldl` reduces a list to a summary value, `unfoldl` builds a
    //! list from a seed value and a function. In some cases, `unfoldl` can
    //! undo a `foldl` operation:
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
    //! @param f
    //! A function called as `f(init)`, where `init` is an initial value,
    //! and returning
    //!     1. `nothing` if it is done producing the list.
    //!     2. `just(pair(init, x))` if it isn't, where `init` is the new
    //!        initial value used in the next call to `f` and `x` is an
    //!        element to be appended to the resulting list. Also note
    //!        that `pair` may actually be replaced by any instance of
    //!        the `Product` type class.
    //!
    //! @param init
    //! An initial value to build the list from.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp unfoldl
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename L>
    constexpr auto unfoldl = [](auto&& f, auto&& init) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L>
    struct _unfoldl {
        template <typename F, typename Initial>
        constexpr decltype(auto) operator()(F&& f, Initial&& initial) const {
            return List::instance<L>::unfoldl_impl(
                detail::std::forward<decltype(f)>(f),
                detail::std::forward<decltype(initial)>(initial)
            );
        }
    };

    template <typename L>
    constexpr _unfoldl<L> unfoldl{};
#endif

    //! Dual to `foldr` for lists.
    //! @relates List
    //!
    //! While `foldr` reduces a list to a summary value, `unfoldr` builds a
    //! list from a seed value and a function. In some cases, `unfoldr` can
    //! undo a `foldr` operation:
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
    //! @param f
    //! A function called as `f(init)`, where `init` is an initial value,
    //! and returning
    //!     1. `nothing` if it is done producing the list.
    //!     2. `just(pair(x, init))` if it isn't, where `init` is the new
    //!        initial value used in a recursive call to `f` and `x` is an
    //!        element prepended to the resulting list. Also note that `pair`
    //!        may actually be replaced by any instance of the `Product` type
    //!        class.
    //!
    //! @param init
    //! An initial value to build the list from.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp unfoldr
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename L>
    constexpr auto unfoldr = [](auto&& f, auto&& init) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L>
    struct _unfoldr {
        template <typename F, typename Initial>
        constexpr decltype(auto) operator()(F&& f, Initial&& initial) const {
            return List::instance<L>::unfoldr_impl(
                detail::std::forward<decltype(f)>(f),
                detail::std::forward<decltype(initial)>(initial)
            );
        }
    };

    template <typename L>
    constexpr _unfoldr<L> unfoldr{};
#endif


    //! Unzip a list of lists.
    //! @relates List
    //!
    //! Specifically, takes a list of the form
    //! @code
    //!     [s1, s2, ..., sn]
    //! @endcode
    //! where each `si` is a list, and returns a list equivalent to
    //! `zip(s1, s2, ..., sn)`.
    //!
    //!
    //! @param xs
    //! A list of lists to unzip.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp unzip
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto unzip = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _unzip {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return List::instance<
                datatype_t<Xs>
            >::unzip_impl(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _unzip unzip{};
#endif

    //! Zip one list or more.
    //! @relates List
    //!
    //! This is equivalent to `zip_with(make<L>, xs, ys...)`, where `L` is
    //! the data type of `xs`.
    //!
    //! @note
    //! All the lists must have the same data type, and only the data type
    //! of the first list is used for dispatching the method.
    //!
    //! @param xs, ys...
    //! The lists to zip together.
    //!
    //! ### Example
    //! @snippet example/list.cpp zip
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto zip = [](auto&& xs, auto&& ...ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _zip {
        template <typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ...ys) const {
            return List::instance<
                datatype_t<Xs>
            >::zip_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)...
            );
        }
    };

    constexpr _zip zip{};
#endif

    //! Zip one list or more with a given function.
    //! @relates List
    //!
    //! Specifically, returns a list whose i-th element is `f(s1[i], ..., sn[i])`,
    //! where `sk[i]` denotes the i-th element of the k-th list passed as an
    //! argument. The returned list stops when the shortest input sequence is
    //! exhausted.
    //!
    //! In other words, `zip_with(f, s1, ..., sn)` is a list of the form
    //! @code
    //!     f(s1[0], ..., sn[0])
    //!     f(s1[1], ..., sn[1])
    //!     ...
    //!     f(s1[k], ..., sn[k])
    //! @endcode
    //! where `k` is the length of the shortest list.
    //!
    //! @note
    //! All the lists must have the same data type, and only the data type
    //! of the first list is used for dispatching the method.
    //!
    //!
    //! @param f
    //! A function called as `f(s1[k], ..., sn[k])`, where `si[k]` are
    //! elements of the zipped lists as explained above.
    //!
    //! @param xs, ys...
    //! The lists to zip with the given function.
    //!
    //!
    //! ### Example
    //! @snippet example/list.cpp zip_with
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/zip_with.ctime.png
    //!
    //! @todo
    //! Consider allowing only two lists and achieving the variadic behavior
    //! in some other way. This would make it possible to automatically curry
    //! `zip_with`. It might be possible to achieve the variadic behavior with
    //! e.g. Applicative Functors?
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto zip_with = [](auto&& f, auto&& xs, auto&& ...ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _zip_with {
        template <typename F, typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(F&& f, Xs&& xs, Ys&& ...ys) const {
            return List::instance<
                datatype_t<Xs>
            >::zip_with_impl(
                detail::std::forward<F>(f),
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)...
            );
        }
    };

    constexpr _zip_with zip_with{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_LIST_HPP
