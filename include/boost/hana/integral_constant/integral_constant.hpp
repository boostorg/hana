/*!
@file
Forward declares `boost::hana::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `IntegralConstant` type class is used for `Constant`s holding a
    //! compile-time value of an integral type.
    //!
    //! ### Requires
    //! `Constant`, `Comparable`, `Orderable`, `Logical`,
    //! `Monoid`, `Group`, `Ring`, and `IntegralDomain`
    //!
    //! ### Laws
    //! For any two `IntegralConstant`s `x` and `y` of data types `X` and `Y`
    //! respectively, the following must hold:
    //! @code
    //!     x == y
    //!     if and only if
    //!     integral_constant<X, decltype(value(x)), value(x)> == integral_constant<Y, decltype(value(y)), value(y)>
    //! @endcode
    //!
    //! This law ensures that an `IntegralConstant` can't carry more information
    //! than what can be passed to `integral_constant`. Additionally, all
    //! superclass instances are required to be isomorphic to the instances
    //! for the `Integral` data type. Together, these requirements ensure that
    //! all `IntegralConstant`s are pretty unsurprising and are basically
    //! equivalent to the `Integral` data type, except perhaps with a
    //! different implementation.
    struct IntegralConstant {
        BOOST_HANA_TYPECLASS(IntegralConstant);
        struct mcd;
    };

    //! Create an `IntegralConstant` of the given data type and holding a
    //! value of the given integral type.
    //! @relates IntegralConstant
    //!
    //! Specifically, `integral_constant<I, T, v>` is an `IntegralConstant`
    //! of data type `I` with an underlying value `v` of the integral type `T`.
    //!
    //!
    //! @tparam I
    //! The data type of the created `IntegralConstant`.
    //!
    //! @tparam T
    //! The integral type of the value held in the `IntegralConstant`.
    //!
    //! @tparam v
    //! The integral value held in the `IntegralConstant`.
    //!
    //!
    //! ### Example
    //! @snippet example/integral_constant.cpp integral_constant
    template <typename I, typename T, T v>
    BOOST_HANA_CONSTEXPR_LAMBDA auto integral_constant =
        IntegralConstant::instance<I>::
        template integral_constant_impl<T, v>();
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP
