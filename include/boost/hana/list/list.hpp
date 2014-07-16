/*!
@file
Forward declares `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_LIST_HPP
#define BOOST_HANA_LIST_LIST_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp> // for instantiates and convert
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>
#include <boost/hana/traversable/traverse_mcd.hpp>

#include <type_traits> // for std::is_same


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
        static_assert(std::is_same<
            datatype_t<decltype(xs)>, datatype_t<decltype(ys)>
        >::value, "");
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

    //! @details
    //! `List`s implement `fmap` as the mapping of a function over each
    //! element of the list. This is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //!
    //! ### Example 1
    //! @snippet example/list/functor/fmap.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/functor/fmap.cpp main
    template <typename T>
    struct Functor::instance<T, when<instantiates<List, T>()>>
        : Functor::fmap_mcd
    {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            auto cons_f = [=](auto x, auto xs) { return cons(f(x), xs); };
            return foldr(cons_f, nil<T>, xs);
        }
    };

    //! @details
    //! A value can be lifted into a singleton list with `lift`. `ap(fs, xs)`
    //! applies each function in the list `fs` to each value in the list `xs`,
    //! and returns a list containing all the results.
    //!
    //! ### Example
    //! @snippet example/list/applicative/overview.cpp main
    template <typename T>
    struct Applicative::instance<T, when<instantiates<List, T>()>>
        : Applicative::mcd
    {
        template <typename X>
        static constexpr auto lift_impl(X x)
        { return cons(x, nil<T>); }

        template <typename Fs, typename Xs>
        static constexpr auto ap_impl(Fs fs, Xs xs)
        { return bind(fs, [=](auto f) { return fmap(f, xs); }); }
    };

    //! @details
    //! A function returning a list of results can be mapped over all the
    //! elements of a list and have all the results concatenated using `bind`.
    //! Also, a list of lists can be flattened one level with `flatten`.
    //!
    //! ### Example
    //! @snippet example/list/monad/overview.cpp main
    template <typename T>
    struct Monad::instance<T, when<instantiates<List, T>()>>
        : Monad::flatten_mcd
    {
        template <typename Xss>
        static constexpr auto flatten_impl(Xss xss)
        { return foldl(concat, nil<T>, xss); }
    };

    //! @details
    //! This instance is hard to describe in words; see the examples.
    //!
    //! ### Example
    //! @snippet example/list/traversable/traverse.cpp main
    //!
    //! ### Example
    //! @snippet example/list/traversable/sequence.cpp main
    template <typename T>
    struct Traversable::instance<T, when<instantiates<List, T>()>>
        : Traversable::traverse_mcd
    {
        template <typename A, typename F, typename Xs>
        static constexpr auto traverse_impl(F f, Xs xs) {
            auto curried_cons = [](auto x) {
                return [=](auto xs) { return cons(x, xs); };
            };
            auto cons_f = [=](auto x, auto ys) {
                return ap(fmap(curried_cons, f(x)), ys);
            };
            return foldr(cons_f, lift<A>(nil<T>), xs);
        }
    };

    //! @details
    //! Two `List`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //!
    //! ### Example
    //! @snippet example/list/comparable.cpp main
    template <typename T, typename U>
    struct Comparable::instance<T, U, when<
        instantiates<List, T>() && instantiates<List, U>()
    >> : Comparable::equal_mcd
    {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys) {
            return eval_if(or_(is_empty(xs), is_empty(ys)),
                [=](auto _) {
                    return and_(_(is_empty)(xs), _(is_empty)(ys));
                },
                [=](auto _) {
                    return and_(equal(_(head)(xs), _(head)(ys)),
                                equal_impl(_(tail)(xs), _(tail)(ys)));
                }
            );
        }
    };

    //! Converts a `Foldable` to a `List`.
    template <typename L, typename T>
    struct convert<L, T, detail::enable_if_t<
        instantiates<List, L>() && instantiates<Foldable, T>()
    >> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return foldr(cons, nil<L>, xs); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_LIST_HPP
