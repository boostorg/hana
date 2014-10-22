/*!
@file
Defines `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_HPP
#define BOOST_HANA_INTEGRAL_HPP

#include <boost/hana/fwd/integral.hpp>

// instances
#include <boost/hana/constant.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    template <>
    struct Constant::instance<Integral> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <>
    struct IntegralConstant::instance<Integral> : IntegralConstant::mcd {
        template <typename T, T v>
        static constexpr auto integral_constant_impl()
        { return integral<T, v>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
