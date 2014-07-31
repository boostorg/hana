/*!
@file
Adapts Boost.MPL IntegralConstants.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP

#include <boost/hana/constant/mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/type_traits.hpp>

#include <boost/mpl/integral_c_tag.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Data type representing Boost.MPL IntegralConstants
    //!
    //! ### Instance of
    //! `Constant`
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    struct MplIntegralConstant { };
#else
    using MplIntegralConstant = boost::mpl::integral_c_tag;
#endif

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<typename T::tag, MplIntegralConstant>::value
    >> {
        using type = MplIntegralConstant;
    };

    //! `Constant` instance for Boost.MPL IntegralConstants.
    //!
    //! ### Example
    //! @snippet example/ext/boost/mpl/integral_c/constant.cpp main
    template <>
    struct Constant::instance<MplIntegralConstant> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_INTEGRAL_C_HPP
