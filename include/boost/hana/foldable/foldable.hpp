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
        struct mcd;
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_FOLDABLE_HPP
