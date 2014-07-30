/*!
@file
Forward declares `boost::hana::Orderable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_ORDERABLE_HPP
#define BOOST_HANA_ORDERABLE_ORDERABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Orderable` type class is used for data types defining a
    //! [strict weak ordering](http://en.wikipedia.org/wiki/Strict_weak_ordering).
    //!
    //! @anchor strict_weak_ordering
    //! ### Laws
    //! `less` must define a strict weak ordering. Formally, let
    //!
    //! @code
    //!     x ~ y if and only if !(x < y) && !(y < x)
    //! @endcode
    //!
    //! be the incomparability relation. Then, for all `a`, `b`, `c` of an
    //! orderable data type,
    //!
    //! @code
    //!     !(a < a)                     // Irreflexivity
    //!     if a < b then !(b < a)       // Asymmetry
    //!     if a < b && b < c then a < c // Transitivity
    //!     if a ~ b && b ~ c then a ~ c // Transitivity of incomparability
    //! @endcode
    struct Orderable {
        BOOST_HANA_BINARY_TYPECLASS(Orderable);
        struct less_mcd;
        struct laws;
    };

    //! Returns a `Logical` representing whether `x` is less than `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable/less.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto less = [](auto x, auto y) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::less_impl(x, y);
    };

    //! Returns a `Logical` representing whether `x` is less than or
    //! equal to `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable/less_equal.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto less_equal = [](auto x, auto y) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::less_equal_impl(x, y);
    };

    //! Returns a `Logical` representing whether `x` is greater than `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable/greater.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto greater = [](auto x, auto y) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::greater_impl(x, y);
    };

    //! Returns a `Logical` representing whether `x` is greater than or
    //! equal to `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable/greater_equal.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto greater_equal = [](auto x, auto y) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::greater_equal_impl(x, y);
    };

    //! Returns the smallest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable/min.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto min = [](auto x, auto y) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::min_impl(x, y);
    };

    //! Returns the greatest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable/max.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto max = [](auto x, auto y) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::max_impl(x, y);
    };

    //! Returns a function performing `less` after applying a transformation
    //! to both arguments.
    //! @relates Orderable
    //!
    //! @note
    //! This is not a method of the `Orderable` type class, but just a
    //! convenience function provided with it.
    //!
    //! ### Example
    //! @snippet example/orderable/ordering.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto ordering = [](auto f) {
        return [=](auto x, auto y) {
            return less(f(x), f(y));
        };
    };

    namespace operators {
        //! Equivalent to `less`.
        //! @relates boost::hana::Orderable
        template <typename T, typename U>
        constexpr auto operator<(T t, U u)
        { return less(t, u); }

        //! Equivalent to `less_equal`.
        //! @relates boost::hana::Orderable
        template <typename T, typename U>
        constexpr auto operator<=(T t, U u)
        { return less_equal(t, u); }

        //! Equivalent to `greater`.
        //! @relates boost::hana::Orderable
        template <typename T, typename U>
        constexpr auto operator>(T t, U u)
        { return greater(t, u); }

        //! Equivalent to `greater_equal`.
        //! @relates boost::hana::Orderable
        template <typename T, typename U>
        constexpr auto operator>=(T t, U u)
        { return greater_equal(t, u); }
    }

    //! Minimal complete definition: `less`
    struct Orderable::less_mcd {
        template <typename X, typename Y>
        static constexpr auto less_equal_impl(X x, Y y)
        { return not_(less(y, x)); }

        template <typename X, typename Y>
        static constexpr auto greater_impl(X x, Y y)
        { return less(y, x); }

        template <typename X, typename Y>
        static constexpr auto greater_equal_impl(X x, Y y)
        { return not_(less(x, y)); }

        template <typename X, typename Y>
        static constexpr auto min_impl(X x, Y y)
        { return if_(less(x, y), x, y); }

        template <typename X, typename Y>
        static constexpr auto max_impl(X x, Y y)
        { return if_(less(x, y), y, x); }
    };

    //! Instance of `Orderable` for objects with `LessThanComparable` data
    //! types.
    //!
    //! Any two objects whose data types can be compared using `operator<`
    //! are automatically instances of `Orderable` by using that comparison.
    template <typename X, typename Y>
    struct Orderable::instance<X, Y, when_valid<decltype((void)(*(X*)0 < *(Y*)0))>>
        : Orderable::less_mcd
    {
        static constexpr auto less_impl(X x, Y y)
        { return x < y; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_ORDERABLE_HPP
