/*!
@file
Defines the instance of `boost::hana::Orderable` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_ORDERABLE_HPP
#define BOOST_HANA_STRING_ORDERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/orderable/less_mcd.hpp>
#include <boost/hana/string/string.hpp>


namespace boost { namespace hana {
    //! Instance of `Orderable` for `String`s.
    //!
    //! The strict weak ordering implemented for `Orderable` is the usual
    //! lexicographical comparison.
    //!
    //! ### Example
    //! @snippet example/string/orderable.cpp main
    template <>
    struct Orderable::instance<String, String> : Orderable::less_mcd {
    private:
        static constexpr bool strless(char const* s1, char const* s2) {
            while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
                ++s1, ++s2;

            return (*s1 == '\0' && *s2 != '\0') || // s1 is shorter than s2
                   (*s1 != '\0' && *s2 != '\0' && *s1 < *s2); // s1[0] < s2[0]
        }

    public:
        template <typename S1, typename S2>
        static constexpr auto less_impl(S1 const&, S2 const&) {
            constexpr char const* s1 = S1::get();
            constexpr char const* s2 = S2::get();
            return bool_<strless(s1, s2)>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_ORDERABLE_HPP
