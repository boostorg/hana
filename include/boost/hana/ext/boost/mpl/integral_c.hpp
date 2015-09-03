/*!
@file
Adapts Boost.MPL IntegralConstants for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/fwd/core/convert.hpp>

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
    // IntegralConstant/Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IntegralConstant<ext::boost::mpl::integral_c_tag<T>> {
        static constexpr bool value = true;
    };

    template <typename T, typename C>
    struct to_impl<ext::boost::mpl::integral_c_tag<T>, C,
        when<hana::IntegralConstant<C>::value>
    > : embedding<is_embedded<typename C::value_type, T>::value> {
        template <typename N>
        static constexpr auto apply(N const&) {
            return ::boost::mpl::integral_c<T, N::value>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
