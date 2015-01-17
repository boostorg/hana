/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
using namespace boost::hana;


//////////////////////////////////////////////////////////////////////////////
// 1. `equal_impl` is not provided when the objects share a common type but
//    that common type isn't Comparable
//////////////////////////////////////////////////////////////////////////////
namespace tc1 { struct T { }; struct U { }; struct C { }; }
namespace boost { namespace hana {
    template <>
    struct common<tc1::T, tc1::U> {
        using type = tc1::C;
    };

    template <>
    struct convert<tc1::C, tc1::T> {
        static constexpr tc1::C apply(tc1::T) { return {}; }
    };
    template <>
    struct convert<tc1::C, tc1::U> {
        static constexpr tc1::C apply(tc1::U) { return {}; }
    };

    template <>
    struct equal_impl<tc1::T, tc1::T>
    { static constexpr bool apply(tc1::T, tc1::T) { return true; } };
    template <>
    struct equal_impl<tc1::U, tc1::U>
    { static constexpr bool apply(tc1::U, tc1::U) { return true; } };
}}
BOOST_HANA_CONSTANT_CHECK(not_(is_implemented<equal_impl<tc1::T, tc1::U>>));


//////////////////////////////////////////////////////////////////////////////
// 2. Two objects of unrelated data types are unequal by default.
//////////////////////////////////////////////////////////////////////////////
namespace tc2 {
    struct T { }; struct U { };
    BOOST_HANA_CONSTANT_CHECK(is_implemented<equal_impl<T, U>>);
    BOOST_HANA_CONSTANT_CHECK(not_(equal(T{}, U{})));
}

//////////////////////////////////////////////////////////////////////////////
// 3. Two objects of the same data type are not comparable by default.
//////////////////////////////////////////////////////////////////////////////
namespace tc3 {
    struct T { };
    BOOST_HANA_CONSTANT_CHECK(not_(is_implemented<equal_impl<T, T>>));
}

//////////////////////////////////////////////////////////////////////////////
// 4. Comparable objects sharing a Comparable common type can be compared.
//////////////////////////////////////////////////////////////////////////////
namespace tc4 { struct T { }; struct U { }; struct C { }; }
namespace boost { namespace hana {
    template <>
    struct common<tc4::T, tc4::U> {
        using type = tc4::C;
    };

    template <>
    struct convert<tc4::C, tc4::T> {
        static constexpr tc4::C apply(tc4::T) { return {}; }
    };
    template <>
    struct convert<tc4::C, tc4::U> {
        static constexpr tc4::C apply(tc4::U) { return {}; }
    };

    template <>
    struct equal_impl<tc4::C, tc4::C>
    { static constexpr bool apply(tc4::C, tc4::C) { return true; } };
    template <>
    struct equal_impl<tc4::T, tc4::T>
    { static constexpr bool apply(tc4::T, tc4::T) { return true; } };
    template <>
    struct equal_impl<tc4::U, tc4::U>
    { static constexpr bool apply(tc4::U, tc4::U) { return true; } };
}}
BOOST_HANA_CONSTANT_CHECK(is_implemented<equal_impl<tc4::T, tc4::U>>);


int main() { }
