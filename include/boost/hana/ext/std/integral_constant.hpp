/*!
@file
Defines `boost::hana::ext::std::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/fwd/ext/std/integral_constant.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/integral_constant.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(ext::std::IntegralConstant<T>)>
        : detail::std::true_type
    { };

    template <typename T>
    struct value_impl<ext::std::IntegralConstant<T>> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<ext::std::IntegralConstant<T>, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to a std::integral_constant of a "
        "non-integral type; std::integral_constant may only hold "
        "integral types");

        template <typename X>
        static constexpr auto apply(X x) {
            constexpr auto v = hana::value(x);
            return ::std::integral_constant<T, static_cast<T>(v)>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // IntegralConstant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IntegralConstant::instance<ext::std::IntegralConstant<T>>
        : IntegralConstant::mcd
    {
        template <T v>
        static constexpr auto integral_constant_impl()
        { return ::std::integral_constant<T, v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
