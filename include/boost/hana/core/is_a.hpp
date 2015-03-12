/*!
@file
Defines `boost::hana::is_a` and `boost::hana::is_an`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_IS_A_HPP
#define BOOST_HANA_CORE_IS_A_HPP

#include <boost/hana/fwd/core/is_a.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/is_same.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // is_a
    //////////////////////////////////////////////////////////////////////////
    template <typename DataType, typename T>
    struct _is_a<DataType, T>
        : _integral_constant<bool,
            detail::std::is_same<
                DataType, typename datatype<T>::type
            >{}
        >
    { };

    template <typename DataType>
    struct _is_a<DataType> {
        template <typename T>
        constexpr auto operator()(T const&) const
        { return is_a<DataType, T>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_IS_A_HPP
