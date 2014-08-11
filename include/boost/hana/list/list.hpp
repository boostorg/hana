/*!
@file
Forward declares `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_LIST_HPP
#define BOOST_HANA_LIST_LIST_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/type_traits.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! @ingroup group-datatypes
    //!
    //! General purpose index-based sequence.
    //!
    //!
    //! ### Laws
    //! For any two `List`s `xs` and `ys`, the following statement must hold:
    //!
    //! @code
    //!     xs == ys if and only if to<List>(xs) == to<List>(ys)
    //! @endcode
    //!
    //! This is basically saying that all `List` instances are isomorphic to
    //! the instance defined by the `List` data type, and it therefore makes
    //! sense to define comparison for any two instances of `List`.
    //!
    //!
    //! @todo
    //! - It might be possible to optimize the implementation of homogeneous
    //!   lists using an array.
    //! - How to implement iterate and repeat?
    //! - We could provide automatic unit testing for any instance because we
    //!   have the isomorphisms.
    //! - There is a strong relationship between this and `MonadPlus`.
    //!   Actually, they might be just the same. Check this out.
    //! - Implement the following methods:
    //!     - `intersperse`, `intercalate`, `transpose`, `subsequences`
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

    //! Concatenate two lists together.
    //! @relates List
    //!
    //! @param xs, ys
    //! Two instances of `List` with _the same data type_.
    //!
    //! ### Example
    //! @snippet example/list/concat.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto concat = [](auto&& xs, auto&& ys) -> decltype(auto) {
        static_assert(detail::std::is_same<
            datatype_t<decltype(xs)>, datatype_t<decltype(ys)>
        >::value,
        "boost::hana::concat: both arguments must have the same data type");
        return List::instance<
            datatype_t<decltype(xs)>
        >::concat_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(ys)>(ys)
        );
    };

    //! Prepend an element to the head of a list.
    //! @relates List
    //!
    //!
    //! @param x
    //! An element to prepend to the list.
    //!
    //! @param xs
    //! The list to which an element is prepended.
    //!
    //!
    //! ### Example
    //! @snippet example/list/cons.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto&& x, auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::cons_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(xs)>(xs)
        );
    };

    //! Return a list containing only the elements satisfying a `predicate`.
    //! @relates List
    //!
    //!
    //! @param xs
    //! The list to filter.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` in the list
    //! and returning a `Logical` representing whether that element should be
    //! __kept__ in the resulting list. In the current version of the library,
    //! the `predicate` has to return a [compile-time](@ref Logical_terminology)
    //! `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list/filter.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::filter_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(predicate)>(predicate)
        );
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
    //! @snippet example/list/group_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto group_by = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::group_by_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(xs)>(xs)
        );
    };

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
    //! @snippet example/list/group.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto group = [](auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::group_impl(detail::std::forward<decltype(xs)>(xs));
    };

    //! Remove the last element of a non-empty list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/init.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto init = [](auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::init_impl(detail::std::forward<decltype(xs)>(xs));
    };

    namespace list_detail {
        template <typename T>
        struct into {
            template <typename ...Xs>
            constexpr decltype(auto) operator()(Xs&& ...xs) const {
                return List::instance<T>::into_impl(detail::std::forward<Xs>(xs)...);
            }
        };
    }

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
    //! in the resulting list in the same order as passed to `into`.
    //!
    //!
    //! ### Example
    //! @snippet example/list/into.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename L>
    constexpr auto into = [](auto&& ...xs) -> decltype(auto) {
        return List::instance<L>::into_impl(std::forward<decltype(xs)>(xs)...);
    };
#else
    template <typename L>
    constexpr list_detail::into<L> into{};
#endif

    //! `nil<L>` is an empty list of data type `L`.
    //! @relates List
    //!
    //! @tparam L
    //! The data type of the empty list wanted. This must be an instance of
    //! the `List` type class.
    //!
    //!
    //! ### Example
    //! @snippet example/list/nil.cpp main
    template <typename L>
    BOOST_HANA_CONSTEXPR_LAMBDA auto nil = List::instance<L>::nil_impl();

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
    //! @snippet example/list/partition.cpp ints
    //!
    //! ### Example
    //! @snippet example/list/partition.cpp types
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::partition_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(predicate)>(predicate)
        );
    };

    //! Return a list of all the permutations of the given list.
    //! @relates List
    //!
    //! The permutations are not guaranteed to be in any specific order.
    //!
    //! @note
    //! Implementation taken from http://stackoverflow.com/a/2184129/627587.
    //!
    //! ### Example
    //! @snippet example/list/permutations.cpp main
    //!
    //! ### Benchmarks
    //! @image html benchmark.list.permutations.time.png
    //!
    //! @bug
    //! We got a performance problem here. Generating the permutations of
    //! a list of more than 3 elements takes a long time (>6s).
    BOOST_HANA_CONSTEXPR_LAMBDA auto permutations = [](auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::permutations_impl(detail::std::forward<decltype(xs)>(xs));
    };

    //! Reverse a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/reverse.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::reverse_impl(detail::std::forward<decltype(xs)>(xs));
    };

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
    //! @snippet example/list/scanl.cpp main
    //!
    //! ### Benchmarks
    //! @image html benchmark.list.scanl.time.png
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanl = [](auto&& xs, auto&& state, auto&& f) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanl_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(state)>(state),
            detail::std::forward<decltype(f)>(f)
        );
    };

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
    //! @snippet example/list/scanl1.cpp main
    //!
    //! ### Benchmarks
    //! @image html benchmark.list.scanl1.time.png
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanl1 = [](auto&& xs, auto&& f) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanl1_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(f)>(f)
        );
    };

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
    //! @snippet example/list/scanr.cpp main
    //!
    //! ### Benchmarks
    //! @image html benchmark.list.scanr.time.png
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanr = [](auto&& xs, auto&& state, auto&& f) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanr_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(state)>(state),
            detail::std::forward<decltype(f)>(f)
        );
    };

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
    //! @snippet example/list/scanr1.cpp main
    //!
    //! ### Benchmarks
    //! @image html benchmark.list.scanr1.time.png
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanr1 = [](auto&& xs, auto&& f) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanr1_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(f)>(f)
        );
    };

    //! Append an element to the end of a list.
    //! @relates List
    //!
    //!
    //! @param xs
    //! The list to which an element is appended.
    //!
    //! @param x
    //! An element to append to the list.
    //!
    //!
    //! ### Example
    //! @snippet example/list/snoc.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto&& xs, auto&& x) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::snoc_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(x)>(x)
        );
    };

    //! Sort a list based on the `less` [strict weak ordering](@ref strict_weak_ordering).
    //! @relates List
    //!
    //! The elements in the list must all be `Orderable`. In the current
    //! version of the library, the `less` method of the `Orderable`
    //! instance must be a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/list/sort.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort = [](auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::sort_impl(detail::std::forward<decltype(xs)>(xs));
    };

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
    //! @snippet example/list/sort_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort_by = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::sort_by_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(xs)>(xs)
        );
    };

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
    //!                             take_while(predicate, xs),
    //!                             drop_while(predicate, xs)
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
    //! @snippet example/list/span.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto span = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::span_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(predicate)>(predicate)
        );
    };

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
    //! @snippet example/list/take.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take = [](auto&& n, auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::take_impl(
            detail::std::forward<decltype(n)>(n),
            detail::std::forward<decltype(xs)>(xs)
        );
    };

    //! Take elements until the `predicate` is satisfied.
    //! @relates List
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements do not satisfy the predicate. This is effectively equivalent
    //! to `take_while` with a negated predicate.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! list, and returning a `Logical` representing whether the resulting
    //! list should stop at the element before `x`. In the current version
    //! of the library, `predicate` has to return a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! @param xs
    //! The list to take the elements from.
    //!
    //!
    //! ### Example
    //! @snippet example/list/take_until.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_until = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::take_until_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(xs)>(xs)
        );
    };

    //! Take elements while the `predicate` is satisfied.
    //! @relates List
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements satisfy the given predicate.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! list, and returning a `Logical` representing whether `x` should be
    //! included in the resulting list. In the current version of the library,
    //! `predicate` has to return a [compile-time](@ref Logical_terminology)
    //! `Logical`.
    //!
    //! @param xs
    //! The list to take elements from.
    //!
    //!
    //! ### Example
    //! @snippet example/list/take_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_while = [](auto&& predicate, auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::take_while_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(xs)>(xs)
        );
    };

    namespace list_detail {
        template <typename L>
        struct unfoldr {
            template <typename F, typename Initial>
            constexpr decltype(auto) operator()(F&& f, Initial&& initial) const {
                return List::instance<L>::unfoldr_impl(
                    detail::std::forward<decltype(f)>(f),
                    detail::std::forward<decltype(initial)>(initial)
                );
            }
        };

        template <typename L>
        struct unfoldl {
            template <typename F, typename Initial>
            constexpr decltype(auto) operator()(F&& f, Initial&& initial) const {
                return List::instance<L>::unfoldl_impl(
                    detail::std::forward<decltype(f)>(f),
                    detail::std::forward<decltype(initial)>(initial)
                );
            }
        };
    }

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
    //! @snippet example/list/unfoldl.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename L>
    constexpr auto unfoldl = [](auto&& f, auto&& init) -> decltype(auto) {
        return List::instance<L>::unfoldl_impl(
            std::forward<decltype(f)>(f),
            std::forward<decltype(init)>(init)
        );
    };
#else
    template <typename L>
    constexpr list_detail::unfoldl<L> unfoldl{};
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
    //! @snippet example/list/unfoldr.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename L>
    constexpr auto unfoldr = [](auto&& f, auto&& init) -> decltype(auto) {
        return List::instance<L>::unfoldr_impl(
            std::forward<decltype(f)>(f),
            std::forward<decltype(init)>(init)
        );
    };
#else
    template <typename L>
    constexpr list_detail::unfoldr<L> unfoldr{};
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
    //! @snippet example/list/unzip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto unzip = [](auto&& xs) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::unzip_impl(detail::std::forward<decltype(xs)>(xs));
    };

    //! Zip one list or more.
    //! @relates List
    //!
    //! This is equivalent to `zip_with(into<L>, xs, ys...)`, where `L` is
    //! the data type of `xs`.
    //!
    //! @param xs, ys...
    //! The lists to zip together.
    //!
    //! ### Example
    //! @snippet example/list/zip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto&& xs, auto&& ...ys) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::zip_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(ys)>(ys)...
        );
    };

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
    //! @snippet example/list/zip_with.cpp main
    //!
    //! @todo
    //! Consider allowing only two lists and achieving the variadic behavior
    //! in some other way. This would make it possible to automatically curry
    //! `zip_with`. It might be possible to achieve the variadic behavior with
    //! e.g. Applicative Functors?
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto&& f, auto&& xs, auto&& ...ys) -> decltype(auto) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::zip_with_impl(
            detail::std::forward<decltype(f)>(f),
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(ys)>(ys)...
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_LIST_HPP
