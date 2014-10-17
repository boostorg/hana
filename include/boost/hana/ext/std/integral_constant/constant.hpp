/*!
@file
Defines the instance of `boost::hana::Constant` for
`boost::hana::StdIntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_CONSTANT_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_CONSTANT_HPP

#include <boost/hana/constant/mcd.hpp>
#include <boost/hana/ext/std/integral_constant/fwd.hpp>


namespace boost { namespace hana {
    template <>
    struct Constant::instance<StdIntegralConstant> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };
}}

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_CONSTANT_HPP
