/*!
@file
Defines `boost::hana::datatype` and `boost::hana::datatype_t`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DATATYPE_HPP
#define BOOST_HANA_CORE_DATATYPE_HPP

#include <boost/hana/fwd/core/datatype.hpp>

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    template <typename T, typename>
    struct datatype : datatype<T, when<true>> { };

    template <typename T, bool condition>
    struct datatype<T, when<condition>> {
        using type = T;
    };

    template <typename T>
    struct datatype<T, when_valid<typename T::hana::datatype>> {
        using type = typename T::hana::datatype;
    };

    template <typename T> struct datatype<T const> : datatype<T> { };
    template <typename T> struct datatype<T volatile> : datatype<T> { };
    template <typename T> struct datatype<T const volatile> : datatype<T> { };
    template <typename T> struct datatype<T&> : datatype<T> { };
    template <typename T> struct datatype<T&&> : datatype<T> { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DATATYPE_HPP
