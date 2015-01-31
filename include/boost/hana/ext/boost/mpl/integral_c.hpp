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

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/detail/std/is_same.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/integral_c_tag.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // datatype
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<typename T::tag, ::boost::mpl::integral_c_tag>{}
    >> {
        using type = ext::boost::mpl::IntegralC<
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
    struct value_impl<ext::boost::mpl::IntegralC<T>> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return C::value; }
    };

    template <typename T, typename C>
    struct convert<ext::boost::mpl::IntegralC<T>, C, when<
        models<Constant(C)>{} && detail::std::is_integral<typename C::value_type>{}
    >> : embedding_if<is_embedding<convert<T, typename C::value_type>>> {
        template <typename X>
        static constexpr auto apply(X)
        { return ::boost::mpl::integral_c<T, value2<X>()>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
