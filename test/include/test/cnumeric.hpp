/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
        template <typename T>
        struct CNumeric { using value_type = T; };

        template <typename T, T v>
        struct cnumeric_type {
            static constexpr T value = v;
            struct hana { using datatype = CNumeric<T>; };

            //! @todo Find a way to make this explicit.
            constexpr operator bool() const { return value; }
        };

        template <typename T, T v>
        constexpr cnumeric_type<T, v> cnumeric{};
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
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
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
