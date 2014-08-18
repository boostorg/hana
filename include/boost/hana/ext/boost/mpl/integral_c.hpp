/*!
@file
Adapts Boost.MPL IntegralConstants.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/constant/mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/integral_constant/mcd.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/integral_c_tag.hpp>

// Goodies
#include <boost/hana/comparable/integral_constant_mcd.hpp>
#include <boost/hana/orderable/integral_constant_mcd.hpp>
#include <boost/hana/logical/integral_constant_mcd.hpp>

#include <boost/hana/monoid/integral_constant_mcd.hpp>
#include <boost/hana/group/integral_constant_mcd.hpp>
#include <boost/hana/ring/integral_constant_mcd.hpp>
#include <boost/hana/integral_domain/integral_constant_mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Data type representing Boost.MPL IntegralConstants
    //!
    //! ### Instance of
    //! `Constant`
    struct BoostMplIntegralConstant;

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename T::tag,
            boost::mpl::integral_c_tag
        >::value
    >> {
        using type = BoostMplIntegralConstant;
    };

    //! `Constant` instance for Boost.MPL IntegralConstants.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/integral_c/constant.cpp
    template <>
    struct Constant::instance<BoostMplIntegralConstant> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <>
    struct IntegralConstant::instance<BoostMplIntegralConstant>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return boost::mpl::integral_c<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
