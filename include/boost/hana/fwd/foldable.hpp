/*!
@file
Forward declares `boost::hana::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FOLDABLE_HPP
#define BOOST_HANA_FWD_FOLDABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Foldable` concept represents data structures that can be folded,
    //! i.e. summarized into a single value.
    //!
    //! Another way of seeing `Foldable`s is as data structures supporting
    //! internal iteration with the ability to accumulate a result. Also
    //! note that since C++ only supports eager evaluation, all models of
    //! `Foldable` must represent finite data structures.
    //!
    //! Additionally, only structures whose total size is known at
    //! compile-time can be models of `Foldable`. This is because of
    //! the `unpack` method, whose return _type_ depends on the number
    //! and types of the objects in the structure.
    //!
    //! @note
    //! While the fact that `Foldable` only works for finite structures may
    //! seem overly restrictive in comparison to the Haskell definition of
    //! `Foldable`, a finer grained separation of the concepts (see `Iterable`
    //! and `Searchable`) should mitigate the issue.
    //!
    //!
    //! Laws
    //! ----
    //! For any `Foldable` `xs`, the following laws must be satisfied:
    //! @code
    //!     foldl(xs, s, f) == foldl(to<Tuple>(xs), s, f)
    //!     foldr(xs, s, f) == foldr(to<Tuple>(xs), s, f)
    //! @endcode
    //!
    //! where `s` and `f` are an arbitrary state and function that can be
    //! used for folding, respectively. Intuitively, these laws say that
    //! `Foldable` respects the left-to-right order of elements within a
    //! structure.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! (`foldl` and `foldr`) or `unpack`
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. Builtin arrays
    //! Builtin arrays whose size is known can be folded as-if they were
    //! homogeneous tuples. However, note that builtin arrays can't be made
    //! more than `Foldable` (e.g. `Iterable`) because they can't be empty
    //! and they also can't be returned from functions.
    //!
    //! @todo
    //! Make it possible to specify the Monoid used for `sum` and `product`?
    struct Foldable { };

    //! Left-associative fold of a structure using a binary operation.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp foldl
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/foldl.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldl = [](auto&& foldable, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldl_impl;

    struct _foldl {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            return foldl_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldl foldl{};
#endif

    //! Right-associative fold of a structure using a binary operation.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp foldr
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/foldr.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldr = [](auto&& foldable, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldr_impl;

    struct _foldr {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            return foldr_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldr foldr{};
#endif

    //! Variant of `foldr` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp foldr1
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/foldr1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldr1 = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldr1_impl;

    struct _foldr1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return foldr1_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldr1 foldr1{};
#endif

    //! Variant of `foldl` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp foldl1
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/foldl1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldl1 = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldl1_impl;

    struct _foldl1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return foldl1_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldl1 foldl1{};
#endif

    //! Perform an action on each element of a foldable, discarding
    //! the result each time.
    //! @relates Foldable
    //!
    //! Iteration is done from left to right, i.e. in the same order as when
    //! using `foldl`. If the structure is not finite, this method will not
    //! terminate.
    //!
    //!
    //! @param foldable
    //! The structure to iterate over.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the structure.
    //! The result of `f(x)`, whatever it is, is ignored.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp for_each
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/for_each.ctime.png
    //!
    //! @todo
    //! The presence of implicit side effects in this function might be a
    //! smell that it should be moved to a different type class and handled
    //! through `Monad`s.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto for_each = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct for_each_impl;

    struct _for_each {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return for_each_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _for_each for_each{};
#endif

    //! Return the number of elements in a finite structure.
    //! @relates Foldable
    //!
    //! ### Example
    //! @snippet example/foldable.cpp length
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/length.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto length = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct length_impl;

    struct _length {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return length_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _length length{};
#endif

    //! Return the least element of a non-empty structure with respect to
    //! a `predicate`.
    //! @relates Foldable
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)`, where `x` and `y` are elements
    //! of the structure. `predicate` should be a [strict weak ordering](@ref strict_weak_ordering)
    //! on the elements of the structure and its return value should be a `Logical`.
    //!
    //! @param foldable
    //! The structure to find the least element of.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp minimum_by
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/minimum_by.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto minimum_by = [](auto&& predicate, auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct minimum_by_impl;

    struct _minimum_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
            return minimum_by_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _minimum_by minimum_by{};
#endif

    //! Return the least element of a non-empty structure.
    //! @relates Foldable
    //!
    //! ### Example
    //! @snippet example/foldable.cpp minimum
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/minimum.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto minimum = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct minimum_impl;

    struct _minimum {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return minimum_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _minimum minimum{};
#endif

    //! Return the greatest element of a non-empty structure with respect to
    //! a `predicate`.
    //! @relates Foldable
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)`, where `x` and `y` are elements
    //! of the structure. `predicate` should be a [strict weak ordering](@ref strict_weak_ordering)
    //! on the elements of the structure and its return value should be a `Logical`.
    //!
    //! @param foldable
    //! The structure to find the greatest element of.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp maximum_by
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/maximum_by.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maximum_by = [](auto&& predicate, auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct maximum_by_impl;

    struct _maximum_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
            return maximum_by_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _maximum_by maximum_by{};
#endif

    //! Return the greatest element of a non-empty structure.
    //! @relates Foldable
    //!
    //! ### Example
    //! @snippet example/foldable.cpp maximum
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/maximum.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maximum = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct maximum_impl;

    struct _maximum {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return maximum_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _maximum maximum{};
#endif

    //! Compute the sum of the numbers of a structure.
    //! @relates Foldable
    //!
    //! It must be possible to perform `plus` on any two adjacent elements of
    //! the structure. The meaning of "adjacent" as used here is that two
    //! elements of the structure `x` and `y` are adjacent if and only if
    //! they are adjacent in `to<Tuple>(foldable)`.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp sum
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/sum.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sum = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct sum_impl;

    struct _sum {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return sum_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _sum sum{};
#endif

    //! Compute the product of the numbers of a structure.
    //! @relates Foldable
    //!
    //! It must be possible to perform `mult` on any two adjacent elements of
    //! the structure. The meaning of "adjacent" as used here is that two
    //! elements of the structure `x` and `y` are adjacent if and only if
    //! they are adjacent in `to<Tuple>(foldable)`.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp product
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/product.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto product = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct product_impl;

    struct _product {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return product_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _product product{};
#endif

    //! Return the number of elements in the structure for which the
    //! `predicate` is satisfied.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure whose elements are counted.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! add `1` to the overall result.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp count
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/count.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto count = [](auto&& foldable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct count_impl;

    struct _count {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return count_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _count count{};
#endif

    //! Invoke a function with the elements of a structure as arguments.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to expand into the function.
    //!
    //! @param f
    //! A function to be invoked as `f(x...)`, where `x...` are the elements
    //! of the structure as-if they had been linearized with `to<Tuple>`.
    //!
    //!
    //! @note
    //! This can be used to transform a function taking multiple arguments
    //! to a function taking a `Foldable` by partially applying `unpack`:
    //! @snippet example/foldable.cpp unpack_idiom
    //! This idiom serves the role of `boost::fusion::make_fused` and
    //! related utilities in the Boost.Fusion library.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable.cpp unpack
    //!
    //! ### Benchmarks
    //! @image html benchmark/foldable/unpack.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto unpack = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct unpack_impl;

    struct _unpack {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return unpack_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _unpack unpack{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FOLDABLE_HPP
