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
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_integral.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // datatype
    //////////////////////////////////////////////////////////////////////////
    namespace std_ic_detail {
        template <typename T, T v>
        constexpr bool
        is_std_integral_constant(::std::integral_constant<T, v>*)
        { return true; }

        constexpr bool is_std_integral_constant(...)
        { return false; }
    }

    template <typename T>
    struct datatype<T, when<std_ic_detail::is_std_integral_constant((T*)0)>> {
        using type = ext::std::IntegralConstant<
            typename T::value_type
        >;
    };

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(ext::boost::mpl::IntegralC<T>)>
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
        models<Constant(C)>{} && detail::std::is_integral<typename C::value_type>{}
    >> : embedding<is_embedded<typename C::value_type, T>> {
        template <typename X>
        static constexpr auto apply(X)
        { return ::std::integral_constant<T, value2<X>()>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGRAL_CONSTANT_HPP
