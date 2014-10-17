/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_COMPARABLE_HPP
#define BOOST_HANA_STRING_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/string/string.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `String`s.
    //!
    //! Two `String`s are equal if and only if each of their
    //! characters are equal.
    //!
    //! ### Example
    //! @snippet example/string/comparable.cpp main
    template <>
    struct Comparable::instance<String, String> : Comparable::equal_mcd {
    private:
        static constexpr bool strequal(char const* s1, char const* s2) {
            while (*s1 != '\0' && *s2 != '\0')
                if (*s1++ != *s2++)
                    return false;
            return *s1 == '\0' && *s2 == '\0';
        }

    public:
        template <typename S1, typename S2>
        static constexpr auto equal_impl(S1 const&, S2 const&) {
            constexpr char const* s1 = S1::get();
            constexpr char const* s2 = S2::get();
            return bool_<strequal(s1, s2)>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_COMPARABLE_HPP
