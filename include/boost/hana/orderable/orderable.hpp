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
#include <boost/hana/detail/std/forward.hpp>


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
        template <typename I1, typename I2>
        struct integral_constant_mcd;
    };

    //! Returns a `Logical` representing whether `x` is less than `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp less
    BOOST_HANA_CONSTEXPR_LAMBDA auto less = [](auto&& x, auto&& y) -> decltype(auto) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::less_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Returns a `Logical` representing whether `x` is less than or
    //! equal to `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp less_equal
    BOOST_HANA_CONSTEXPR_LAMBDA auto less_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::less_equal_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Returns a `Logical` representing whether `x` is greater than `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp greater
    BOOST_HANA_CONSTEXPR_LAMBDA auto greater = [](auto&& x, auto&& y) -> decltype(auto) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::greater_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Returns a `Logical` representing whether `x` is greater than or
    //! equal to `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp greater_equal
    BOOST_HANA_CONSTEXPR_LAMBDA auto greater_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::greater_equal_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Returns the smallest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp min
    BOOST_HANA_CONSTEXPR_LAMBDA auto min = [](auto&& x, auto&& y) -> decltype(auto) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::min_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Returns the greatest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp max
    BOOST_HANA_CONSTEXPR_LAMBDA auto max = [](auto&& x, auto&& y) -> decltype(auto) {
        return Orderable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::max_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Returns a function performing `less` after applying a transformation
    //! to both arguments.
    //! @relates Orderable
    //!
    //! This is not a method of the `Orderable` type class, but just a
    //! convenience function provided with it. Also note that
    //! @code
    //!     ordering(f) == less ^on^ f
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/orderable.cpp ordering
    BOOST_HANA_CONSTEXPR_LAMBDA auto ordering = [](auto&& f) -> decltype(auto) {
        return [f(detail::std::forward<decltype(f)>(f))](auto&& x, auto&& y) -> decltype(auto) {
            return less(
                f(detail::std::forward<decltype(x)>(x)),
                f(detail::std::forward<decltype(y)>(y))
            );
        };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_ORDERABLE_HPP
