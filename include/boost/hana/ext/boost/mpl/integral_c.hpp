/*!
@file
Defines `boost::hana::ext::boost::mpl::IntegralC`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/fwd/ext/boost/mpl/integral_c.hpp>

// instances
#include <boost/hana/constant.hpp>
#include <boost/hana/integral_constant.hpp>

#include <boost/mpl/integral_c.hpp>


namespace boost { namespace hana {
    //! `Constant` instance for Boost.MPL IntegralConstants.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/integral_c/constant.cpp
    template <>
    struct Constant::instance<ext::boost::mpl::IntegralC> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <>
    struct IntegralConstant::instance<ext::boost::mpl::IntegralC>
        : IntegralConstant::mcd
    {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return ::boost::mpl::integral_c<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
