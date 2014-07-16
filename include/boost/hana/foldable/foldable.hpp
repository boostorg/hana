/*!
@file
Forward declares `boost::hana::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_FOLDABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Data structures that can be folded, i.e. summarized into
    //! a single value.
    struct Foldable {
        BOOST_HANA_TYPECLASS(Foldable);
        struct lazy_foldr_mcd;
        struct laws;
    };

    //! Left-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/foldl.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/foldable/foldl.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl = [](auto f, auto state, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldl_impl(f, state, foldable);
    };

    //! Right-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/foldr.cpp main
    //!
    //! ### Example 2
    //! @snippet example/integer_list/foldable/foldr.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr = [](auto f, auto state, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldr_impl(f, state, foldable);
    };

    //! Variant of `foldr` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/foldr1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr1 = [](auto f, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldr1_impl(f, foldable);
    };

    //! Variant of `foldl` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/foldl1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl1 = [](auto f, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::foldl1_impl(f, foldable);
    };

    //! Lazy right-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! Unlike for strict folds, the binary operation should take nullary
    //! functions returning an element and the state instead of taking an
    //! element and the state directly.
    //!
    //! ### Example
    //! @snippet example/range/foldable/lazy_foldr.cpp main
    //!
    //! @bug
    //! We got a performance problem here. It seems very hard to implement
    //! this method efficiently.
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy_foldr = [](auto f, auto state, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::lazy_foldr_impl(f, state, foldable);
    };

    //! Lazy left-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! Unlike for strict folds, the binary operation should take nullary
    //! functions returning the state and an element instead of taking the
    //! state and an element directly.
    //!
    //! @warning
    //! It is important to be aware that a lazy left fold must still walk the
    //! whole structure before it can return. This is because of the nature of
    //! left-folds, which are always equivalent to
    //! @code
    //!     foldl(f, state, structure):
    //!         if (some_stop_condition)
    //!             return state
    //!         else
    //!             return foldl(f, f(...), ...);
    //! @endcode
    //! Notice how `foldl` calls itself recursively in the `else` branch; this
    //! means that the next invocation of `foldl` is always needed, and so the
    //! whole structure has to be unfolded. When `f` is lazy, this has the
    //! effect of creating a (potentially huge) chain of "thunks":
    //! @code
    //!     f(f(f(f(f(x1, state), x2), x3), x4), x5)
    //! @endcode
    //! This chain is then only evaluated lazily, but creating the chain
    //! itself can cause a stack overflow. If you don't need to accumulate
    //! the result lazily, consider using `foldl` instead, which does not
    //! create a chain of thunks and evaluates `f` as it goes.
    //!
    //! ### Example
    //! @snippet example/range/foldable/lazy_foldl.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy_foldl = [](auto f, auto state, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::lazy_foldl_impl(f, state, foldable);
    };

    //! Return the number of elements in a finite structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/length.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto length = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::length_impl(foldable);
    };

    //! Return the least element of a non-empty structure with respect to
    //! a `predicate`.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/type_list/foldable/minimum_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum_by = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::minimum_by_impl(predicate, foldable);
    };

    //! Return the least element of a non-empty structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/integer_list/foldable/minimum.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::minimum_impl(foldable);
    };

    //! Return the largest element of a non-empty structure with respect to
    //! a `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum_by = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::maximum_by_impl(predicate, foldable);
    };

    //! Return the largest element of a non-empty structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::maximum_impl(foldable);
    };

    //! Compute the sum of the numbers of a structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::sum_impl(foldable);
    };

    //! Compute the product of the numbers of a structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/range/foldable/product.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto product = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::product_impl(foldable);
    };

    //! Return the number of elements in the structure for which the
    //! `predicate` is satisfied.
    //! @method{Foldable}
    //!
    //! ### Example 1
    //! @snippet example/integer_list/foldable/count.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/foldable/count.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto count = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::count_impl(predicate, foldable);
    };

    //! Find an element satisfying a predicate in the structure.
    //! @method{Foldable}
    //!
    //! Specifically, returns `just` the first element satisfying the
    //! `predicate`, or `nothing` if there is no such element.
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/find.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/foldable/find.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto find = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::find_impl(predicate, foldable);
    };

    //! Return whether the element occurs in the structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/elem.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto elem = [](auto x, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::elem_impl(x, foldable);
    };

    //! Invoke a function with the elements of a structure as arguments.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/unpack.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto unpack = [](auto f, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::unpack_impl(f, foldable);
    };

    //! Return whether any element of the structure satisfies the `predicate`.
    //! @method{Foldable}
    //!
    //! ### Fusion example
    //! @snippet example/list/foldable/any.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/foldable/any.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto any = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::any_impl(predicate, foldable);
    };

    //! Return whether any element of the structure is true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto any_of = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::any_of_impl(foldable);
    };

    //! Return whether all the elements of the structure satisfy the `predicate`.
    //! @method{Foldable}
    //!
    //! ### Fusion example
    //! @snippet example/list/foldable/all.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/foldable/all.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto all = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::all_impl(predicate, foldable);
    };

    //! Return whether all the elements of the structure are true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto all_of = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::all_of_impl(foldable);
    };

    //! Return whether none of the elements of the structure satisfy the `predicate`.
    //! @method{Foldable}
    //!
    //! ### Fusion example
    //! @snippet example/list/foldable/none.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/foldable/none.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto none = [](auto predicate, auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::none_impl(predicate, foldable);
    };

    //! Return whether none of the elements of the structure are true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto none_of = [](auto foldable) {
        return Foldable::instance<
            datatype_t<decltype(foldable)>
        >::none_of_impl(foldable);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_FOLDABLE_HPP
