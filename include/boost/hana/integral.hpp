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

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>

// instances
#include <boost/hana/constant.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct Constant::instance<Integral<T>> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <typename T>
    struct IntegralConstant::instance<Integral<T>> : IntegralConstant::mcd {
        template <T v>
        static constexpr auto integral_constant_impl()
        { return integral<T, v>; }
    };

    template <typename T, typename C>
    struct convert<Integral<T>, C,
        when<is_an<IntegralConstant, C>()>
    > {
        template <typename X>
        static constexpr decltype(auto) apply(X x) {
            constexpr auto v = value(x);
            return integral<T, v>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
