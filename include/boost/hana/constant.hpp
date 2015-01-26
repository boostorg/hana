/*!
@file
Defines `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_HPP
#define BOOST_HANA_CONSTANT_HPP

#include <boost/hana/fwd/constant.hpp>

#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // value
    //////////////////////////////////////////////////////////////////////////
    template <typename C, typename>
    struct value_impl : value_impl<C, when<true>> { };

    template <typename C, bool condition>
    struct value_impl<C, when<condition>> {
        static_assert(wrong<value_impl<C>>{},
        "no definition of boost::hana::value for the given data type");
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_HPP
