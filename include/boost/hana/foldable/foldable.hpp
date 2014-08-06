/*!
@file
Forward declares `boost::hana::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_FOLDABLE_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Data structures that can be folded, i.e. summarized into
    //! a single value.
    //!
    //! Another way of seeing `Foldable`s is as data structures supporting
    //! internal iteration with the ability to accumulate a result. However,
    //! because C++ only supports eager evaluation, all instances of `Foldable`
    //! must represent finite data structures. While this may seem overly
    //! restrictive in comparison to the Haskell definition of `Foldable`,
    //! a finer grained separation of type classes (see `Iterable` and
    //! `Searchable`) should mitigate the issue.
    struct Foldable {
        BOOST_HANA_TYPECLASS(Foldable);
        struct mcd;
        struct unpack_mcd;
    };

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
    //! @snippet example/foldable/foldl.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl = [](auto foldable, auto state, auto f) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldl_impl(foldable, state, f);
    };

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
    //! @snippet example/foldable/foldr.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr = [](auto foldable, auto state, auto f) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldr_impl(foldable, state, f);
    };

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
    //! @snippet example/foldable/foldr1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr1 = [](auto foldable, auto f) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldr1_impl(foldable, f);
    };

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
    //! @snippet example/foldable/foldl1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl1 = [](auto foldable, auto f) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldl1_impl(foldable, f);
    };

    //! Return the number of elements in a finite structure.
    //! @relates Foldable
    //!
    //! ### Example
    //! @snippet example/foldable/length.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto length = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::length_impl(foldable);
    };

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
    //! @snippet example/foldable/minimum_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum_by = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::minimum_by_impl(predicate, foldable);
    };

    //! Return the least element of a non-empty structure.
    //! @relates Foldable
    //!
    //! ### Example
    //! @snippet example/foldable/minimum.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::minimum_impl(foldable);
    };

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
    //! @snippet example/foldable/maximum_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum_by = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::maximum_by_impl(predicate, foldable);
    };

    //! Return the greatest element of a non-empty structure.
    //! @relates Foldable
    //!
    //! ### Example
    //! @snippet example/foldable/maximum.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::maximum_impl(foldable);
    };

    //! Compute the sum of the numbers of a structure.
    //! @relates Foldable
    //!
    //! It must be possible to perform `operator+` on any two adjacent
    //! elements of the structure. The meaning of "adjacent" as used here
    //! is that two elements of the structure `x` and `y` are adjacent if
    //! and only if they are adjacent in `to<List>(foldable)`.
    //!
    //! ### Example
    //! @snippet example/foldable/sum.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::sum_impl(foldable);
    };

    //! Compute the product of the numbers of a structure.
    //! @relates Foldable
    //!
    //! It must be possible to perform `operator*` on any two adjacent
    //! elements of the structure. The meaning of "adjacent" as used here
    //! is that two elements of the structure `x` and `y` are adjacent if
    //! and only if they are adjacent in `to<List>(foldable)`.
    //!
    //! ### Example
    //! @snippet example/foldable/product.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto product = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::product_impl(foldable);
    };

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
    //! @snippet example/foldable/count.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto count = [](auto foldable, auto predicate) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::count_impl(foldable, predicate);
    };

    //! Invoke a function with the elements of a structure as arguments.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to expand into the function.
    //!
    //! @param f
    //! A function to be invoked as `f(x...)`, where `x...` are the elements
    //! of the structure as-if they had been linearized with `to<List>`.
    //!
    //!
    //! @note
    //! This can be used to transform a function taking multiple arguments
    //! to a function taking a `Foldable` by partially applying `unpack`:
    //! @snippet example/foldable/unpack_idiom.cpp main
    //! This idiom serves the role of `boost::fusion::make_fused` and
    //! related utilities in the Boost.Fusion library.
    //!
    //!
    //! ### Example
    //! @snippet example/foldable/unpack.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto unpack = [](auto foldable, auto f) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::unpack_impl(foldable, f);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_FOLDABLE_HPP
