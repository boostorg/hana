/*!
@file
Defines `boost::hana::ext::boost::mpl::IntegralC`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/integral_c_tag.hpp>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace mpl {
        //! @ingroup group-ext
        //! Adapter for IntegralConstants from the Boost.MPL.
        //!
        //! Provided models
        //! ---------------
        //! 1. `Constant`\n
        //! As Constants holding an integral type, `IntegralC`s are models
        //! not only of Constant, but also all the other concepts that are
        //! provided for Constants of an integral type.
        //! @snippet example/ext/boost/mpl/integral_c.cpp constant
        template <typename T>
        struct IntegralC { using value_type = T; };
    }}}

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename T::tag,
            ::boost::mpl::integral_c_tag
        >::value
    >> {
        using type = ext::boost::mpl::IntegralC<
            typename datatype<typename T::value_type>::type
        >;
    };

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<ext::boost::mpl::IntegralC<T>> {
        template <typename C>
        static constexpr auto apply()
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<ext::boost::mpl::IntegralC<T>, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to a Boost.MPL IntegralConstant of a "
        "non-integral type; Boost.MPL IntegralConstants may only hold "
        "integral types");

        template <typename X>
        static constexpr auto apply(X const&) {
            constexpr T v = hana::value<X>();
            return ::boost::mpl::integral_c<T, v>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
