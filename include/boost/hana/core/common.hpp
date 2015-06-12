/*!
@file
Defines `boost::hana::common` and `boost::hana::common_t`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_COMMON_HPP
#define BOOST_HANA_CORE_COMMON_HPP

#include <boost/hana/fwd/core/common.hpp>

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std_common_type.hpp>
#include <boost/hana/detail/void_t.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // common
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct common : common<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct common<T, U, when<condition>>
        : detail::std_common_type<T, U>
    { };

    template <typename T>
    struct common<T, T> {
        using type = T;
    };

    //////////////////////////////////////////////////////////////////////////
    // has_common
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct has_common : std::false_type { };

    template <typename T, typename U>
    struct has_common<T, U, detail::void_t<typename common<T, U>::type>>
        : std::true_type
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_COMMON_HPP
