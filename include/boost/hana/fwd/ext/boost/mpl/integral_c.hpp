/*!
@file
Forward declares `boost::hana::ext::boost::mpl::IntegralC`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_FWD_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_same.hpp>

#include <boost/mpl/integral_c_tag.hpp>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace mpl {
        //! @ingroup group-datatypes
        //! Data type representing Boost.MPL IntegralConstants.
        //!
        //!
        //! Provided models
        //! ---------------
        //! 1. `Constant`\n
        //! @include example/ext/boost/mpl/integral_c/constant.cpp
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_BOOST_MPL_INTEGRAL_C_HPP
