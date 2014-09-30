/*!
@file
Forward declares `boost::hana::String`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_STRING_HPP
#define BOOST_HANA_STRING_STRING_HPP

#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time string.
    //!
    //! ## Instance of
    //! `Comparable`, `Constant`
    struct String { struct hana_enabled_operators : Comparable { }; };

    //! Create a compile-time string from a string literal `s`.
    //! @relates String
    //!
    //! This macro is the only way to create a compile-time string. Also,
    //! since this macro uses a lambda internally, it can't be used in an
    //! unevaluated context.
    //!
    //! ### Example
    //! @snippet example/string/string.cpp macro
    //!
    //! @todo
    //! This could be generalized to arbitrary objects. This is basically a
    //! way to create a `Constant` from any `constexpr` object.
#define BOOST_HANA_STRING(s)                                                \
    ([] {                                                                   \
        struct tmp : ::boost::hana::operators::enable_adl {                 \
            static constexpr char const* get() { return s; }                \
            using hana_datatype = ::boost::hana::String;                    \
        };                                                                  \
        return tmp{};                                                       \
    }())                                                                    \
/**/
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_STRING_HPP
