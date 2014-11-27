/*!
@file
Forward declares `boost::hana::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `IntegralConstant` type class is used for `Constant`s holding a
    //! compile-time value of an integral type.
    //!
    //! `IntegralConstant` imposes an additional requirement on data types `D`
    //! instantiating the type class. This requirement is that `D` be
    //! parameterized over a type `T` representing the type of the constant
    //! held in an object of data type `D`. In other words, `D` must actually
    //! be a specialization of the form `D<T>`.
    //!
    //! ### Requires
    //! `Constant`, `Comparable`, `Orderable`, `Logical`,
    //! `Monoid`, `Group`, `Ring`, and `IntegralDomain`
    //!
    //! ### Laws
    //! For any two `IntegralConstant`s `x` and `y` of data types `X<T>` and
    //! `Y<U>` respectively, the following must hold:
    //! @code
    //!     x == y
    //!     if and only if
    //!     integral_constant<X<T>, value(x)>() == integral_constant<Y<U>, value(y)>()
    //! @endcode
    //!
    //! This law ensures that an `IntegralConstant` can't carry more information
    //! than what can be passed to `integral_constant`. Additionally, all
    //! superclass instances are required to be isomorphic to the instances
    //! for the `Integral` data type. Together, these requirements ensure that
    //! all `IntegralConstant`s are pretty unsurprising and are basically
    //! equivalent to the `Integral` data type, except perhaps with a
    //! different implementation.
    //!
    //! Additionally, for all `IntegralConstant`s `c` of data type `C<T>`,
    //! the following must be satisfied:
    //! @code
    //!     c == to<C>(i)
    //! @endcode
    //! where `i` is an _arbitrary_ `IntegralConstant` such that
    //! `value(i) == value(c)`. This law ensures that all `IntegralConstant`s
    //! can be converted without loss of information from one to another,
    //! which in turn requires the instances of `IntegralConstant` to be
    //! the same up to implementation differences. This allows the library
    //! to provide several type class instances for free.
    struct IntegralConstant {
        BOOST_HANA_TYPECLASS(IntegralConstant);
        struct mcd;
    };

    //! Create an `IntegralConstant` of the given data type and holding a
    //! value of the given integral type.
    //! @relates IntegralConstant
    //!
    //! Specifically, `integral_constant<C<T>, v>()` is an `IntegralConstant`
    //! of data type `C<T>` with an underlying value `v` of the integral
    //! type `T`.
    //!
    //!
    //! @tparam C<T>
    //! The data type of the created `IntegralConstant`. The integral value
    //! held in the created object has the integral type `T`.
    //!
    //! @tparam v
    //! The integral value held in the `IntegralConstant`.
    //!
    //!
    //! ### Example
    //! @snippet example/integral_constant.cpp integral_constant
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class C, typename T, T v>
    constexpr auto integral_constant<C<T>, v> = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    namespace ic_detail {
        template <typename C>
        struct param;

        template <template <typename ...> class C, typename T>
        struct param<C<T>> { using type = T; };
    }

    template <typename C, typename ic_detail::param<C>::type v>
    struct _integral_constant {
        constexpr decltype(auto) operator()() const {
            return IntegralConstant::instance<C>::
                   template integral_constant_impl<v>();
        }
    };

    template <typename C, typename ic_detail::param<C>::type v>
    constexpr _integral_constant<C, v> integral_constant{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP
