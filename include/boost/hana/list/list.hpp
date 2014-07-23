/*!
@file
Forward declares `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_LIST_HPP
#define BOOST_HANA_LIST_LIST_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/type_traits.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    @ingroup datatypes
    General purpose index-based sequence.

    --------------------------------------------------------------------------

    ## Instance of (as a data type)
    `Iterable`, `Functor`, `Applicative`, `Monad`, `Foldable`, `Traversable`,
    and `Comparable`

    --------------------------------------------------------------------------

    ## Laws (as a type class)
    For any two `List`s `x` and `y`,
    @f{align*}{
        x = y \iff \tt{to<List>(x)} = \tt{to<List>(y)}
    @f}
    This is basically saying that all `List` instances are isomorphic, and
    it therefore makes sense to define comparison for any two `List`s.

    --------------------------------------------------------------------------

    @todo
    - It might be possible to optimize the implementation of homogeneous lists
      using an array.
    - How to implement iterate and repeat?
    - Check laws for `Applicative`.
    - Get rid of the `<type_traits>` include.
    - We could provide automatic unit testing for any instance because we
      have the isomorphisms.
    - There is a strong relationship between this and `MonadPlus`. Actually,
      they might be just the same. Check this out.
    - Implement the following methods:
        - `unfoldr`, `unfoldl`, `unzip`
        - `intersperse`, `intercalate`, `transpose`, `subsequences`
        - `split_at`, `span`, `break`, `group_by`, `group`, `inits`, `tails`
    - Consider implementing the following methods:
        - `nub_by`, `nub`, `delete_by`, `insert`
        - `set_difference_by`, `set_union_by`, `set_intersection_by`
     */
    struct List {
        BOOST_HANA_TYPECLASS(List);
        template <typename T>
        struct mcd;
    };

    //! Concatenate two lists together.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/concat.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto concat = [](auto xs, auto ys) {
        static_assert(detail::std::is_same<
            datatype_t<decltype(xs)>, datatype_t<decltype(ys)>
        >::value,
        "boost::hana::concat: both arguments must have the same data type");
        return List::instance<
            datatype_t<decltype(xs)>
        >::concat_impl(xs, ys);
    };

    //! Prepend an element to the head of a list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/cons.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto x, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::cons_impl(x, xs);
    };

    //! Return a list containing only the elements satisfying the `predicate`.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/filter.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto predicate, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::filter_impl(predicate, xs);
    };

    //! Removes the last element of a non-empty list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/init.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto init = [](auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::init_impl(xs);
    };

    namespace list_detail {
        template <typename T>
        struct into {
            template <typename ...Xs>
            constexpr auto operator()(Xs ...xs) const
            { return List::instance<T>::into_impl(xs...); }
        };
    }

    //! Creates a `List` with the given elements in it.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/into.cpp main
    template <typename T>
    constexpr list_detail::into<T> into{};

    //! An empty list.
    //! @method{List}
    template <typename T>
    BOOST_HANA_CONSTEXPR_LAMBDA auto nil = List::instance<T>::nil_impl();

    //! Partition a list based on a `predicate`.
    //! @method{List}
    //!
    //! Specifically, returns a `Pair` whose first element is a list of the
    //! elements satisfying the predicate, and whose second element is a
    //! list of the elements that do not satisfy the predicate.
    //!
    //! @note
    //! The predicate must return an `Integral`.
    //!
    //! ### Example 1
    //! @snippet example/integer_list/partition.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/partition.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto predicate, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::partition_impl(predicate, xs);
    };

    //! Return a list of all the permutations of the given list.
    //! @method{List}
    //!
    //! The permutations are not guaranteed to be in any specific order.
    //!
    //! ### Example
    //! @snippet example/list/permutations.cpp main
    //!
    //! @note
    //! Implementation taken from http://stackoverflow.com/a/2184129/627587.
    //!
    //! @bug
    //! We got a performance problem here. Generating the permutations of
    //! a list of more than 3 elements starts taking a long time (>6s).
    BOOST_HANA_CONSTEXPR_LAMBDA auto permutations = [](auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::permutations_impl(xs);
    };

    //! Reverse a list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/reverse.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::reverse_impl(xs);
    };

    //! Similar to `foldl`, but returns a list of reduced values from the left.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/scanl.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanl = [](auto f, auto s, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanl_impl(f, s, xs);
    };

    //! Variant of `scanl` that has no starting value argument.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/scanl1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanl1 = [](auto f, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanl1_impl(f, xs);
    };

    //! Similar to `foldr`, but returns a list of reduced values from the right.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/scanr.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanr = [](auto f, auto s, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanr_impl(f, s, xs);
    };

    //! Variant of `scanr` that has no starting value argument.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/scanr1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto scanr1 = [](auto f, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::scanr1_impl(f, xs);
    };

    //! Append an element to the end of a list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/snoc.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto xs, auto x) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::snoc_impl(xs, x);
    };

    //! Sort a list based on the `less` strict weak ordering.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/sort.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort = [](auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::sort_impl(xs);
    };

    //! Sort a list based on the given `predicate`.
    //! @method{List}
    //!
    //! The predicate must be a [strict weak ordering](http://en.wikipedia.org/wiki/Strict_weak_ordering#Strict_weak_orderings).
    //! The sort is guaranteed to be stable.
    //!
    //! ### Example
    //! @snippet example/list/sort_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort_by = [](auto predicate, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::sort_by_impl(predicate, xs);
    };

    //! Return the first `n` elements of a list.
    //! @method{List}
    //!
    //! `n` must be a non-negative `Integral` representing the number of
    //! elements to keep. If `n` is greater than the length of the list,
    //! the whole list is returned.
    //!
    //! ### Example
    //! @snippet example/list/take.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take = [](auto n, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::take_impl(n, xs);
    };

    //! Take elements until the `predicate` is satisfied.
    //! @method{List}
    //!
    //! This is equivalent to `take_while` with a negated predicate.
    //!
    //! ### Example
    //! @snippet example/integer_list/take_until.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_until = [](auto predicate, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::take_until_impl(predicate, xs);
    };

    //! Take elements while the `predicate` is satisfied.
    //! @method{List}
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements satisfy the given predicate. The predicate must return
    //! an `Integral`.
    //!
    //! ### Example
    //! @snippet example/integer_list/take_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_while = [](auto predicate, auto xs) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::take_while_impl(predicate, xs);
    };

    //! Zip one list or more.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/zip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto xs, auto ...ys) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::zip_impl(xs, ys...);
    };

    /*!
    Zip one list or more with a given function.
    @method{List}

    Specifically, returns a list whose i-th element is `f(s1[i], ..., sn[i])`,
    where `sk[i]` denotes the i-th element of the k-th list passed as an
    argument. The returned list stops when the shortest input sequence is
    exhausted.

    In other words, `zip_with(f, s1, ..., sn)` is a list of the form
    @code
        f(s1[0], ..., sn[0])
        f(s1[1], ..., sn[1])
        ...
        f(s1[k], ..., sn[k])
    @endcode
    where `k` is the length of the shortest list.

    ### Example
    @snippet example/list/zip_with.cpp main

    @todo
    - Consider allowing only two lists and achieving the variadic behavior
    in some other way. This would make it possible to automatically curry
    `zip_with`. It might be possible to achieve the variadic behavior with
    e.g. Applicative Functors?
     */
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto xs, auto ...ys) {
        return List::instance<
            datatype_t<decltype(xs)>
        >::zip_with_impl(f, xs, ys...);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_LIST_HPP
