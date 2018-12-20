/*
@file
Defines _HANA_TN_CAN_CONSTEXPR and related macros
(_HANA_TN_CONSTEXPR_IF_POSSIBLE, _HANA_SIZEOF_OR_STRLEN)

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_COMPILER_CAPABILITIES_HPP
#define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_COMPILER_CAPABILITIES_HPP

// only clang and MSVC support constexpr __PRETTY_FUNCTION__, gcc does not
#if defined(__clang__) || defined(_MSC_VER)
    #define _HANA_TN_CAN_CONSTEXPR
#endif

// in constexpr mode, strlen is equivalent to sizeof() - 1
#ifdef _HANA_TN_CAN_CONSTEXPR
    #define _HANA_TN_CONSTEXPR_IF_POSSIBLE constexpr
    #define _HANA_SIZEOF_OR_STRLEN(var) sizeof(var) - 1
#else
    #include <cstring> // this include is not needed in constexpr mode, save compilation time
    #define _HANA_TN_CONSTEXPR_IF_POSSIBLE
    #define _HANA_SIZEOF_OR_STRLEN(var) strlen(var)
#endif

#endif // !BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_COMPILER_CAPABILITIES_HPP
