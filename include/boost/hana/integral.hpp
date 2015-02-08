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

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(Integral<T>)>
        : detail::std::true_type
    { };

    template <typename T>
    struct value_impl<Integral<T>> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<Integral<T>, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to an Integral of a non-integral "
        "type; boost::hana::Integral may only hold integral types");

        template <typename X>
        static constexpr auto apply(X x) {
            constexpr auto v = hana::value(x);
            return integral<T, static_cast<T>(v)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // IntegralConstant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IntegralConstant::instance<Integral<T>> : IntegralConstant::mcd {
        template <T v>
        static constexpr auto integral_constant_impl()
        { return integral<T, v>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
