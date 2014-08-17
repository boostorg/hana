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
    //! @todo
    //! Explain how all IntegralConstants instances are isomorphic.
    struct IntegralConstant {
        BOOST_HANA_TYPECLASS(IntegralConstant);
        struct mcd;
    };

    //! Create an `IntegralConstant` of the given data type and holding a
    //! value of the given integral type.
    //! @relates IntegralConstant
    //!
    //! @todo Document this
    template <typename I, typename T, T v>
    BOOST_HANA_CONSTEXPR_LAMBDA auto integral_constant =
        IntegralConstant::instance<I>::
        template integral_constant_impl<T, v>();
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_CONSTANT_INTEGRAL_CONSTANT_HPP
