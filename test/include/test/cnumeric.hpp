/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_HPP

#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/orderable.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace test {
        template <typename T>
        struct CNumeric { using value_type = T; };

        template <typename T, T v>
        struct cnumeric_t {
            static constexpr T value = v;
            struct hana { using datatype = CNumeric<T>; };
            constexpr operator T() const { return value; }
        };

        template <typename T, T v>
        constexpr cnumeric_t<T, v> cnumeric{};

        template <typename T, T v>
        constexpr cnumeric_t<T, v> make_cnumeric() { return {}; }
    }

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<test::CNumeric<T>> {
        template <typename X>
        static constexpr auto apply()
        { return X::value; }
    };

    template <typename T, typename C>
    struct to_impl<test::CNumeric<T>, C, when<
        _models<Constant, C>::value &&
        std::is_integral<typename C::value_type>::value
    >>
        : embedding<is_embedded<typename C::value_type, T>::value>
    {
        static_assert(std::is_integral<T>{},
        "trying to convert a Constant to a test::CNumeric of a non-integral "
        "type; test::CNumeric may only hold integral types");

        template <typename X>
        static constexpr auto apply(X const&) {
            constexpr T v = hana::value<X>();
            return test::cnumeric<T, v>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct equal_impl<test::CNumeric<T>, test::CNumeric<U>> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return test::cnumeric<bool, X::value == Y::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct less_impl<test::CNumeric<T>, test::CNumeric<U>> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return test::cnumeric<bool, (X::value < Y::value)>; }
    };
}}

#endif // !BOOST_HANA_TEST_TEST_CNUMERIC_HPP
