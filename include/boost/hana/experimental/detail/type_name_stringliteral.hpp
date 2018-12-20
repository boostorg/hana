/*
@file
Defines
    hana::experimental::type_name_details::stringliteral
    _HANA_TN_MAKE_STRINGLITERAL

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_STRINGLITERAL_HPP
#define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_STRINGLITERAL_HPP

#include <boost/hana/experimental/detail/type_name_compiler_capabilities.hpp>

#include <cstddef>
#include <string>
#include <cstring>


namespace boost {
namespace hana {
namespace experimental {
namespace type_name_details {

    struct stringliteral {
        char const* ptr;
        std::size_t length;
    };


#define _HANA_TN_MAKE_STRINGLITERAL(str_literal) stringliteral { str_literal, _HANA_SIZEOF_OR_STRLEN(str_literal);


    inline constexpr std::size_t constexpr_strlen(char const * s) {
        std::size_t r = 0;
        while(*s++ != '\0')
            r++;
        return r;
    }

    inline constexpr bool stringliteral_equal(stringliteral const & cs1, stringliteral const & cs2) {
        if (cs1.length != cs2.length)
            return false;

        std::size_t idx = 0;
        do {
            if (cs1.ptr[idx] != cs2.ptr[idx])
                return false;
            idx++;
        } while (idx < cs1.length);
        return true;
    }

    inline constexpr bool stringliteral_equal_sz(stringliteral const & cs1, char const * literal) {
        return stringliteral_equal(
            cs1,
            { literal, constexpr_strlen(literal) }
        );
    }

    inline std::string stringliteral_to_string(stringliteral const & cs) {
        return std::string(cs.ptr, cs.length);
    }

} // namespace type_name_details
} // namespace experimental
} // namespace hana
} // namespace boost

#endif // !BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_STRINGLITERAL_HPP
