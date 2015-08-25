/*!
@file
Adapts Boost.MPL IntegralConstants for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/fwd/core/convert.hpp>
#include <boost/hana/value.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/integral_c_tag.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace mpl {
        //! @ingroup group-ext
        //! Adapter for IntegralConstants from the Boost.MPL.
        //!
        //! Provided models
        //! ---------------
        //! 1. `Constant`\n
        //! As Constants holding an integral type, `mpl::integral_c`s are
        //! models not only of Constant, but also all the other concepts
        //! that are provided for Constants of an integral type.
        //! @snippet example/ext/boost/mpl/integral_c.cpp constant
        template <typename T>
        struct integral_c_tag { using value_type = T; };
    }}}

    template <typename T>
    struct tag_of<T, when<
        std::is_same<
            typename T::tag,
            ::boost::mpl::integral_c_tag
        >::value
    >> {
        using type = ext::boost::mpl::integral_c_tag<
            typename hana::tag_of<typename T::value_type>::type
        >;
    };

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<ext::boost::mpl::integral_c_tag<T>> {
        template <typename C>
        static constexpr auto apply()
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<ext::boost::mpl::integral_c_tag<T>, C, when<
        Constant<C>::value &&
        std::is_integral<typename C::value_type>::value
    >>
        : embedding<is_embedded<typename C::value_type, T>::value>
    {
        static_assert(std::is_integral<T>{},
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
